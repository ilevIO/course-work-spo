#include "r123.h"
#include "ui_r123.h"
#include <QMouseEvent>
#include <QDebug>
#include "controller/r123controller.h"
#include <sstream>
#include <random>
#include <time.h>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <math.h>
#include <stdlib.h>

int countFreq(QLabel *lab1, QLabel *lab2)
{
    int lab1val = int(atof(lab1->text().toStdString().c_str()) * 1000);
    int lab2val = int(atof(lab2->text().toStdString().c_str()) * 1000);
    //489
    int deltaLab1 = lab1->geometry().x() - 489;
    int deltaLab2 = lab2->geometry().x() - 489;

    int resVal, resDelta;

    if (deltaLab1 < 0 && deltaLab2 < 0)
    {
        resDelta = abs(deltaLab1 > deltaLab2 ? deltaLab1 : deltaLab2);
        resVal = deltaLab1 > deltaLab2 ? lab1val : lab2val;
    }
    else if (deltaLab1 > 0 && deltaLab2 < 0)
    {
        resDelta = abs(deltaLab2);
        resVal = lab2val;
    }
    else if (deltaLab1 < 0 && deltaLab2 > 0)
    {
        resDelta = abs(deltaLab1);
        resVal = lab1val;
    }
    else
    {
        resDelta = deltaLab1 < deltaLab2 ? deltaLab1 : deltaLab2;
        resVal = deltaLab1 < deltaLab2 ? lab1val : lab2val;
        resDelta -= 96;
        resDelta = abs(resDelta);
        resVal -= 100;
    }
    //96
    qDebug() << resDelta;
    if (resDelta < 12)
    {
        return resVal - 25;
    }
    else if (resDelta >= 12 && resDelta < 36)
    {
        return resVal;
    }
    else if (resDelta >= 36 && resDelta < 60)
    {
        return resVal + 25;
    }
    else if (resDelta >= 84)
    {
        return resVal + 75;
    }
    else
    {
        return resVal + 50;
    }

}

void R123::wheelEvent(QWheelEvent *event)
{

    if (this->ui->tabWidget->currentIndex() == 1)
    {
        this->keyReleaseEvent(nullptr);
        static int angle_us_ch = 0, angle_vrt = 0;
        static QPixmap * vsua_nast_ch = new QPixmap(":/res/r123/vsua_naptr_kr.png");
        static QPixmap * vsua_vrt = new QPixmap(":/res/r123/vsua_vrt.png");
        if (event->pos().rx() > 276 && event->pos().ry() > 344
                && event->pos().rx() < 331 && event->pos().ry() < 397)
        {
            QPixmap pixmap(*vsua_nast_ch);
            QMatrix rm;

            if (event->angleDelta().ry() > 0)
                angle_us_ch += 4;
            else if (event->angleDelta().ry() < 0)
                angle_us_ch -= 4;

            if (angle_us_ch > 360)
                angle_us_ch -= 360;
            if (angle_us_ch < -360)
                angle_us_ch += 360;

            rm.rotate(angle_us_ch);

            pixmap = pixmap.transformed(rm);

            ui->vsua_ustcz->setPixmap(QPixmap(pixmap.transformed(rm)));
        }
        if (event->pos().rx() > 694 && event->pos().ry() > 374 &&
                event->pos().rx() < 762 && event->pos().ry() < 435)
        {
            QPixmap pixmap(*vsua_vrt);
            QMatrix rm;

            if (event->angleDelta().ry() > 0)
                angle_vrt += 4;
            else if (event->angleDelta().ry() < 0)
                angle_vrt -= 4;

            if (angle_vrt > 360)
                angle_vrt -= 360;
            if (angle_vrt < -360)
                angle_vrt += 360;

            rm.rotate(angle_vrt);

            pixmap = pixmap.transformed(rm);

            ui->vsua_vrt->setPixmap(QPixmap(pixmap.transformed(rm)));
        }
    }
    else if (this->ui->tabWidget->currentIndex() == 0 && this->r123_ziemLaunched)
    {
        static QPixmap *szum = new QPixmap(":/res/r123/r123-szum.png");
        static QPixmap *ust_cz = new QPixmap(":/res/r123/r123-ust_cz.png");
        static QPixmap *nastr = new QPixmap(":/res/r123/r123_nastr.png");
        static QPixmap *volume = new QPixmap(":/res/r123/r123_volume.png");
        static int angle_szum = 0, angle_ust_cz = 0,
                 angle_volume = 0;
        qDebug() << event->x() << " " << event->y();
        if (event->x() > 179 && event->y() > 136
                && event->x() < 253 && event->y() < 198)
        {
            this->keyReleaseEvent(nullptr);
            QPixmap pixmap(*szum);
            QMatrix rm;
            if (event->angleDelta().ry() > 0 && angle_szum <= 8)
                angle_szum += 4;
            else if (event->angleDelta().ry() < 0 && angle_szum >= -72)
                angle_szum -= 4;

            if (angle_szum > 360)
                angle_szum -= 360;
            if (angle_szum < -360)
                angle_szum += 360;
            rm.rotate(angle_szum);

            pixmap = pixmap.transformed(rm);

            ui->r123_szum->setPixmap(QPixmap(pixmap.transformed(rm)));
        }
        else if (event->x() > 255 && event->y() > 264
                 && event->x() < 358 && event->y() < 360 && this->r123_ustcz_can_move) {
            this->keyReleaseEvent(nullptr);
            static int substep = 3;
            static int step = 20;
            QPixmap pixmap(*ust_cz);
            QMatrix rm;
            int position = 0;
            if (event->angleDelta().ry() > 0)
            { angle_ust_cz += 4; position = 4; }
            else if (event->angleDelta().ry() < 0)
            { angle_ust_cz -= 4; position = -4; }

            if (this->ui->r123_number_poddiap1_1->text() == "20.0" && position > 0 && this->ui->r123_number_poddiap1_1->pos().x() >= 460)
            { angle_ust_cz -= 4; return; }
            if (this->ui->r123_number_poddiap2_2->text() == "51.5" && position < 0 && this->ui->r123_number_poddiap2_2->pos().x() <= 460)
            { angle_ust_cz += 4; return; }

            if (angle_ust_cz > 360)
                angle_ust_cz -= 360;
            if (angle_ust_cz < -360)
                angle_ust_cz += 360;
            rm.rotate(angle_ust_cz);

            pixmap = pixmap.transformed(rm);

            ui->r123_ust_cz->setPixmap(QPixmap(pixmap.transformed(rm)));


            QRect rect = this->ui->r123_line->geometry();
            if (rect.x() > 418)
                rect.moveTo(161, rect.y());
            else if (rect.x() < 117)
                rect.moveTo(373, rect.y());
            rect.moveTo(rect.x() + position, rect.y());

            qDebug() << rect.x();

            auto incNumber = [](std::string string){
                if (string[3] >= '8')
                {
                    if (string[1] >= '9')
                    {
                        string[1] = '0';
                        string[3] = string[3] == '9' ? '1' : '0';
                        string[0] += 1;
                    } else {
                        string[3] = string[3] == '9' ? '1' : '0';
                        string[1] += 1;
                    }
                } else string[3] += 2;
                return string;
            };
            auto decNumber = [](std::string string) {
                if (string[3] <= '1')
                {
                    if (string[1] <= '0')
                    {
                        string[1] = '9';
                        string[3] = string[3] == '1' ? '9' : '8';
                        string[0] -= 1;
                    } else {
                        string[3] = string[3] == '1' ? '9' : '8';
                        string[1] -= 1;
                    }
                } else string[3] -= 2;
                return string;
            };



            auto moveRelative = [incNumber, decNumber](QLabel*label, int pos) {
                QRect rect = label->geometry();
                rect.moveTo(rect.x() + pos, rect.y());
                if (rect.x() < 377) { rect.moveTo(rect.x() + 182, rect.y()); label->setText(incNumber(label->text().toStdString()).c_str()); }
                else if (rect.x() > 555) { rect.moveTo(rect.x() - 182, rect.y()); label->setText(decNumber(label->text().toStdString()).c_str()); }

                label->setGeometry(rect);
            };

            moveRelative(this->ui->r123_number_poddiap1_1, position);
            moveRelative(this->ui->r123_number_poddiap1_2, position);
            moveRelative(this->ui->r123_number_poddiap2_1, position);
            moveRelative(this->ui->r123_number_poddiap2_2, position);
            //460
            // 24 48 68 88 112 132 152 176 196

            this->ui->r123_line->setGeometry(rect);

            step -= position;
            if (step > 20) { step -= substep % 3 == 0 ? 24 : 20; substep+=1; substep%=3; substep+=3; }
            else if (step <= 0) { step += substep % 3 == 0 ? 24 : 20; substep-=1; substep%=3; substep+=3; }

            this->currentFrequency = countFrequency();

            this->angle_to_be_setup[this->currentzpcz - 1] = rand() % 360;


            freqUp = countFreq(this->ui->r123_number_poddiap1_1, this->ui->r123_number_poddiap1_2);
            freqDown = countFreq(this->ui->r123_number_poddiap2_1, this->ui->r123_number_poddiap2_2);
            qDebug() << freqUp;
            qDebug() << freqDown;

        }
        else if (event->x() > 629 && event->y() > 188
                 && event->x() < 787 && event->y() < 322)
        {

            QPixmap pixmap(*nastr);
            QMatrix rm;

            if (event->angleDelta().ry() > 0)
                angle_nastr += 4;
            else if (event->angleDelta().ry() < 0)
                angle_nastr -= 4;

            if (angle_nastr > 360)
                angle_nastr -= 720;
            if (angle_nastr < -360)
                angle_nastr += 720;
            rm.rotate(angle_nastr);

            pixmap = pixmap.transformed(rm);

            ui->r123_nastr->setPixmap(QPixmap(pixmap.transformed(rm)));

            updateOpacity();
        }
        else if (event->x() > 869 && event->y() > 389
                 && event->x() < 965 && event->y() < 476)
        {
            this->keyReleaseEvent(nullptr);
            QPixmap pixmap(*volume);
            QMatrix rm;
            if (event->angleDelta().ry() > 0 && angle_volume <= 4)
                angle_volume += 4;
            else if (event->angleDelta().ry() < 0 && angle_volume >= -116)
                angle_volume -= 4;

            if (angle_volume > 360)
                angle_volume -= 360;
            if (angle_volume < -360)
                angle_volume += 360;
            rm.rotate(angle_volume);

            pixmap = pixmap.transformed(rm);

            ui->r123_volume->setPixmap(QPixmap(pixmap.transformed(rm)));
        }
    }
}

void R123::mousePressEvent(QMouseEvent *event)
{
    if(this->ui->tabWidget->currentIndex() == 2)
    {
        this->keyReleaseEvent(nullptr);
        static bool pit_zem = false, pit_pit = false, pit_vsua = false;

        if (event->x() > 271 && event->y() > 472
                && event->x() < 671 && event->y() < 588 && this->BP_active)
        {
            pit_pit = !pit_pit;
            if (pit_pit)
                this->ui->pit_pit->setStyleSheet(
                    "background-image: url(:/res/r123/pit-pit.png);"
                );
            else
                this->ui->pit_pit->setStyleSheet("");
            this->controller.actionResolver(R123StateController::BP_PIT);
        } // power on power
        else if (event->x() > 454 && event->y() > 297
                 && event->x() < 873 && event->y() < 471 && this->BP_active)
        {
            pit_vsua = !pit_vsua;
            if (pit_vsua)
                this->ui->pit_vsua->setStyleSheet("background-image: url(:/res/r123/pit-vsua-pit.png);");
            else
                this->ui->pit_vsua->setStyleSheet("");
            this->controller.actionResolver(R123StateController::BP_PIT_RST);
        }
        else if (event->x() > 181 && event->y() > 484
                 && event->x() < 270 && event->y() < 580)
        {
            pit_zem = !pit_zem;
            if (pit_zem)
                this->ui->pit_zem->setStyleSheet("background-image: url(:/res/r123/pit_zeml.png);");
            else
                this->ui->pit_zem->setStyleSheet("");
            this->controller.actionResolver(R123StateController::BP_ZIEM);
        }

    }
    else if (this->ui->tabWidget->currentIndex() == 1)
    {
        this->keyReleaseEvent(nullptr);
       static bool ukv_connected = false, ukv_ziem = false;
       static bool ukv_ant = false, ukv_ant_launch[4] = {false, false, false, false};

       qDebug() << "x - " << event->x() << " y - " << event->y();

       if (event->x() > 209 && event->y() > 234
               && event->x() < 257 && event->y() < 339)
       {
           ukv_connected = !ukv_connected;
           if (ukv_connected)
               this->ui->vsua_ukv->setStyleSheet("background-image: url(:/res/r123/launch_ukv_vsua.png);");
           else
               this->ui->vsua_ukv->setStyleSheet("");
       }
       else if (event->x() > 265 && event->y() > 472
                && event->x() < 368 && event->y() < 575)
       {
           ukv_ziem = !ukv_ziem;
           if (ukv_ziem)
               this->ui->vsua_ziem->setStyleSheet("background-image: url(:/res/r123/vsua-ziem.png);");
           else
               this->ui->vsua_ziem->setStyleSheet("");
       }
       else if (event->x() > 286 && event->y() > 174
                && event->x() < 340 && event->y() < 223)
       {
           this->ui->vsua_ant2->setStyleSheet("");
           this->ui->vsua_ant3->setStyleSheet("");
           this->ui->vsua_ant4->setStyleSheet("");

           if (ukv_ant_launch[0] == false)
           {
               ukv_ant = true;
               for (int i = 0; i < 4; i++)
                   ukv_ant_launch[i] = false;
               ukv_ant_launch[0] = true;

               this->ui->vsua_ant1->setStyleSheet("background-image: url(:/res/r123/vsua-antenna.png);");
           }
           else
           {
                ukv_ant = false;
                ukv_ant_launch[0] = false;
                this->ui->vsua_ant1->setStyleSheet("");
           }
       }
       else if (event->x() > 372 && event->y() > 174
                && event->x() < 426 && event->y() < 223)
       {
           this->ui->vsua_ant1->setStyleSheet("");
           this->ui->vsua_ant3->setStyleSheet("");
           this->ui->vsua_ant4->setStyleSheet("");

           if (ukv_ant_launch[1] == false)
           {
               ukv_ant = true;
               for (int i = 0; i < 4; i++)
                   ukv_ant_launch[i] = false;
               ukv_ant_launch[1] = true;

               this->ui->vsua_ant2->setStyleSheet("background-image: url(:/res/r123/vsua-antenna.png);");
           }
           else
           {
                ukv_ant = false;
                ukv_ant_launch[1] = false;
                this->ui->vsua_ant2->setStyleSheet("");
           }
       }
       else if (event->x() > 434 && event->y() > 174
                && event->x() < 487 && event->y() < 223)
       {
           this->ui->vsua_ant1->setStyleSheet("");
           this->ui->vsua_ant2->setStyleSheet("");
           this->ui->vsua_ant4->setStyleSheet("");

           if (ukv_ant_launch[2] == false)
           {
               ukv_ant = true;
               for (int i = 0; i < 4; i++)
                   ukv_ant_launch[i] = false;
               ukv_ant_launch[2] = true;

               this->ui->vsua_ant3->setStyleSheet("background-image: url(:/res/r123/vsua-antenna.png);");
           }
           else
           {
                ukv_ant = false;
                ukv_ant_launch[2] = false;
                this->ui->vsua_ant3->setStyleSheet("");
           };
       }
       else if (event->x() > 488 && event->y() > 174
                && event->x() < 533 && event->y() < 223)
       {
           this->ui->vsua_ant1->setStyleSheet("");
           this->ui->vsua_ant2->setStyleSheet("");
           this->ui->vsua_ant3->setStyleSheet("");

           if (ukv_ant_launch[3] == false)
           {
               ukv_ant = true;
               for (int i = 0; i < 4; i++)
                   ukv_ant_launch[i] = false;
               ukv_ant_launch[3] = true;

               this->ui->vsua_ant4->setStyleSheet("background-image: url(:/res/r123/vsua-antenna.png);");
           }
           else
           {
                ukv_ant = false;
                ukv_ant_launch[3] = false;
                this->ui->vsua_ant4->setStyleSheet("");
           }
       }
       else if (event->x() > 648 && event->y() > 281
                && event->x() < 690 && event->y() < 314)
       {
           static int position = 7, napr = -1;
           auto rotate = [this](int angle) {
               static QPixmap pix(":/res/r123/vsua_net.png");
               QPixmap pixmap(pix);
               QMatrix rm;

               rm.rotate(angle);

               pixmap = pixmap.transformed(rm);

               ui->vsua_net->setPixmap(QPixmap(pixmap.transformed(rm)));
           };
           switch(position)
           {
           case 1:
               napr = 1;
               position++;
               rotate(-80);
               break;
           case 2:
               if (napr < 0) {
                   rotate(-92);
                   position--;
               }
               else {
                   position++;
                   rotate(-64);
               }
               break;
           case 3:
               if (napr < 0) {
                   rotate(-80);
                   position--;
               }
               else {
                   position++;
                   rotate(-48);
               }
               break;
           case 4:
               if (napr < 0) {
                   rotate(-64);
                   position--;
               }
               else {
                   position++;
                   rotate(-32);
               }
               break;
           case 5:
               if (napr < 0) {
                   rotate(-48);
                   position--;
               }
               else {
                   position++;
                   rotate(-12);
               }
               break;
           case 6:
               if (napr < 0) {
                   rotate(-32);
                   position--;
               }
               else {
                   position++;
                   rotate(0);
               }
               break;
           case 7:
               rotate(-12);
               napr = -1;
               position--;
               break;
           }
       }
       else if (event->x() > 571 && event->y() > 197
                && event->x() < 615 && event->y() < 237)
       {
           static int position = 3, napr = -1;
           auto rotate = [this](int angle) {
               static QPixmap pix(":/res/r123/vsua_ind.png");
               QPixmap pixmap(pix);
               QMatrix rm;

               rm.rotate(angle);

               pixmap = pixmap.transformed(rm);

               ui->vsua_ind->setPixmap(QPixmap(pixmap.transformed(rm)));
           };
           switch(position)
           {
           case 3:
               napr = -1;
               rotate(-16);
               position--;
               break;
           case 2:
               if (napr < 0) {
                   position--;
                   rotate(-32);
               }
               else {
                   position++;
                   rotate(0);
               }
               break;
           case 1:
               position++;
               napr = 1;
               rotate(-16);
               break;
           }
       }
       else if (event->x() > 540 && event->y() > 392
                && event->x() < 588 && event->y() < 433)
       {
           static int position = 3, napr = 1;
           auto rotate = [this](int angle) {
               static QPixmap pix(":/res/r123/vsua_raw.png");
               QPixmap pixmap(pix);
               QMatrix rm;

               rm.rotate(angle);

               pixmap = pixmap.transformed(rm);

               ui->vsua_raw->setPixmap(QPixmap(pixmap.transformed(rm)));
           };
           switch(position)
           {
           case 1:
               napr = 1;
               position++;
               rotate(-16);
               break;
           case 2:
               if (napr > 0) {
                   position++;
                   rotate(0);
               }
               else {
                   position--;
                   rotate(-28);
               }
               break;
           case 3:
               if (napr > 0) {
                   position++;
                   rotate(16);
               }
               else {
                   position --;
                   rotate(-16);
               }
               break;
           case 4:
               if (napr > 0) {
                   position++;
                   rotate(32);
               }
               else {
                   position--;
                   rotate(0);
               }
               break;
           case 5:
               position--;
               napr = -1;
               rotate(16);
               break;
           }
       }
       else if (event->x() > 404 && event->y() > 333
                && event->x() < 459 && event->y() < 388)
       {
           static int position = 2, napr = 1;
           auto rotate = [this](int angle) {
               static QPixmap pix(":/res/r123/vsua_ant.png");
               QPixmap pixmap(pix);
               QMatrix rm;

               rm.rotate(angle);

               pixmap = pixmap.transformed(rm);

               ui->vsua_ant_vrt->setPixmap(QPixmap(pixmap.transformed(rm)));
           };
           switch(position)
           {
           case 1:
               napr = 1;
               position++;
               rotate(0);
               break;
           case 2:
               if (napr > 0) {
                   position++;
                   rotate(16);
               }
               else {
                   position--;
                   rotate(-16);
               }
               break;
           case 3:
               if (napr > 0) {
                   position++;
                   rotate(34);
               }
               else {
                   position --;
                   rotate(0);
               }
               break;
           case 4:
               if (napr > 0) {
                   position++;
                   rotate(52);
               }
               else {
                   position--;
                   rotate(16);
               }
               break;
           case 5:
               position--;
               napr = -1;
               rotate(34);
               break;
           }
       }
       else if (event->x() > 311 && event->y() > 234
                && event->x() < 349 && event->y() < 265)
       {
            static bool is_on = true;
            static QPixmap on(":/res/r123/vsua_switch_ind.jpg.png");
            static QPixmap off(":/res/r123/vsua_switch_ind_r.jpg.png");
            if (is_on) {
                this->ui->vsua_ind_switch->setPixmap(off);
                is_on = false;
            }
            else {
                this->ui->vsua_ind_switch->setPixmap(on);
                is_on = true;
            }
       }
       else if (event->x() > 288 && event->y() > 438
                && event->x() < 322 && event->y() < 464)
       {
           static QPixmap pix(":/res/r123/vsua-nastr_fil_r.png");
           this->vsua_nast_is_pressed = true;
           this->ui->vsua_nastr_fil->setPixmap(pix);
       }
       else if (event->x() > 316 && event->y() > 284
                && event->x() < 344 && event->y() < 304)
       {
           static QPixmap pix(":/res/r123/vsua-clk_ind.png");
           this->vsua_clk_ind_is_pressed = true;
           this->ui->vsua_clk_ind->setPixmap(pix);
       }
    }
    else if (this->ui->tabWidget->currentIndex() == 0)
    {
        QPixmap pixUpped(":/res/r123/vsua_switch_ind.jpg.png");
        QPixmap pixDwned(":/res/r123/vsua_switch_ind_r.jpg.png");
        QPixmap empty;
        static int cz_state = 0;

        if (event->x() > 997 && event->y() > 418
                && event->x() < 1038 && event->y() < 511)
        {
            this->keyReleaseEvent(nullptr);
            this->r123_ziemLaunched = !this->r123_ziemLaunched;
            if (r123_ziemLaunched)
                this->ui->r123_ziem->setStyleSheet("background-image: url(:/res/r123/r123_ziem.png);");
            else
                this->ui->r123_ziem->setStyleSheet("");
        } else if (event->x() > 966 && event->y() > 42
                   && event->x() < 1045 && event->y() < 346 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            static bool antena_launched = false;
            antena_launched = !antena_launched;
            if (antena_launched)
                ui->r123_antena->setStyleSheet("background-image: url(:/res/r123/r123_ant.png);");
            else
                ui->r123_antena->setStyleSheet("");
            controller.actionResolver(R123StateController::R123_ANTENA);
        } else if (event->x() > 131 && event->y() > 232
            && event->x() < 222 && event->y() < 329 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            // -4 8 20 36 52 68 88 96 116 136 148 168
            static int state = 1;
            auto rotate = [this](int angle) {
                static QPixmap pix(":/res/r123/r123_control_ui.png");
                QPixmap pixmap(pix);
                QMatrix rm;

                rm.rotate(angle);

                pixmap = pixmap.transformed(rm);

                ui->r123_controlu->setPixmap(QPixmap(pixmap.transformed(rm)));
            };
            switch(state) {
            case 1:
                if (event->button() == Qt::LeftButton)
                { rotate(12); state = 2; } else { rotate(172); state = 12; }
                break;
            case 2:
                if (event->button() == Qt::LeftButton)
                { rotate(24); state = 3; } else { rotate(0); state = 1; }
                break;
            case 3:
                if (event->button() == Qt::LeftButton)
                { rotate(40); state = 4; } else { rotate(12); state = 2; }
                break;
            case 4:
                if (event->button() == Qt::LeftButton)
                { rotate(56); state = 5; } else { rotate(24); state = 3; }
                break;
            case 5:
                if (event->button() == Qt::LeftButton)
                { rotate(72); state = 6; } else { rotate(40); state = 4; }
                break;
            case 6:
                if (event->button() == Qt::LeftButton)
                { rotate(92); state = 7; } else { rotate(56); state = 5; }
                break;
            case 7:
                if (event->button() == Qt::LeftButton)
                { rotate(100); state = 8; } else { rotate(72); state = 6; }
                break;
            case 8:
                if (event->button() == Qt::LeftButton)
                { rotate(120); state = 9; } else { rotate(92); state = 7; }
                break;
            case 9:
                if (event->button() == Qt::LeftButton)
                { rotate(140); state = 10; } else { rotate(100); state = 8; }
                break;
            case 10:
                if (event->button() == Qt::LeftButton)
                { rotate(152); state = 11; } else { rotate(120); state = 9; }
                break;
            case 11:
                if (event->button() == Qt::LeftButton)
                { rotate(172); state = 12; } else { rotate(140); state = 10; }
                break;
            case 12:
                if (event->button() == Qt::LeftButton)
                { rotate(0); state = 1; } else { rotate(152); state = 11; }
                break;
            }
            if (state == 2)
                regim_raboty = 1;
            else if (state == 3)
                regim_raboty = 2;
            else if (state == 8)
                regim_raboty = 0;
            else
                regim_raboty = 3;
        }
        else if (event->x() > 387 && event->y() > 237
                 && event->x() < 596 && event->y() < 480 && r123_ziemLaunched) {
            this->keyReleaseEvent(nullptr);
            if (cz_state == 0)
            {
                static QPixmap pix(":/res/r123/open_detail/r123_cz_otkr.png");
                this->ui->r123_cz->setPixmap(pix);
                cz_state = 1;
            } else if (cz_state == 1)
            {
                static QPixmap pix(":/res/r123/open_detail/r123_cz_open.png");
                this->ui->r123_cz->setPixmap(pix);
                cz_state = 2;
            } else if (cz_state == 3)
            {
                static QPixmap pix(":/res/r123/open_detail/r123_cz_zakr.png");
                this->ui->r123_cz->setPixmap(pix);
                cz_state = 0;
            }
        } else if (event->x() > 23 && event->y() > 464
                   && event->x() < 216 && event->y() < 631 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            static bool launched = false;
            if (launched) {
                this->ui->r123_pit->setStyleSheet("");
                launched = false;
            } else {
                this->ui->r123_pit->setStyleSheet("background-image: url(:/res/r123/r123_pit.png);");
                launched = true;
            }
            this->controller.actionResolver(R123StateController::R123_PIT);
        } else if (event->x() > 241 && event->y() > 463
                   && event->x() < 340 && event->y() < 631 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            static bool launched = false;
            if (launched) {
                this->ui->r123_mtg->setStyleSheet("");
                launched = false;
            } else {
                this->ui->r123_mtg->setStyleSheet("background-image: url(:/res/r123/r123_mtg.png);");
                launched = true;
            }
        } else if (event->x() > 115 && event->y() > 353
                   && event->x() < 146 && event->y() < 380 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            //szkala
            static bool launched = false;
            static QPixmap pix(":/res/r123/vsua-clk_ind.png");
            static QPixmap pix_r(":/res/r123/vsua-clk_ind_r.png");
            if (launched)
            {
                this->ui->r123_szkalaswitch->setPixmap(pix);
                launched = !launched;
            } else {
                this->ui->r123_szkalaswitch->setPixmap(pix_r);
                launched = !launched;
            }
            this->controller.actionResolver(R123StateController::R123_SZKALA);
        } else if (event->x() > 194 && event->y() > 353
                   && event->x() < 224 && event->y() < 380 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            //rst vkl
            static bool launched = false;
            static QPixmap pix(":/res/r123/vsua-clk_ind.png");
            static QPixmap pix_r(":/res/r123/vsua-clk_ind_r.png");
            if (launched)
            {
                this->ui->r123_powerswitcher->setPixmap(pix);
                launched = !launched;
            } else {
                this->ui->r123_powerswitcher->setPixmap(pix_r);
                launched = !launched;
            }
            this->controller.actionResolver(R123StateController::R123_POWER);
        } else if (event->x() > 150 && event->y() > 353
                   && event->x() < 190 && event->y() < 380 && r123_ziemLaunched)
        {
            //ton call
            static QPixmap pix(":/res/r123/vsua-nastr_fil_r.png");
            this->ui->r123_ton_call->setPixmap(pix);
            this->r123_ton_call_pressed = true;
        } else if (event->x() > 628 && event->y() > 347
                   && event->x() < 824 && event->y() < 537 && r123_ziemLaunched) {
            static int state = 1;
            this->keyReleaseEvent(nullptr);
            auto rotate = [this](int angle) {
                static QPixmap pix(":/res/r123/r123_zpcz.png");
                QPixmap pixmap(pix);
                QMatrix rm;

                rm.rotate(angle);

                pixmap = pixmap.transformed(rm);

                ui->r123_zpcz->setPixmap(QPixmap(pixmap.transformed(rm)));
            };
            switch (state) {
            case 1:
                if (event->button() == Qt::LeftButton)
                {rotate(30); state = 2; this->controller.setR123Zpcz(1); currentzpcz = 1;} else { rotate(150); state = 6;this->controller.setR123Zpcz(5); currentzpcz = 5;}
                break;
            case 2:
                if (event->button() == Qt::LeftButton)
                {rotate(60); state = 3;this->controller.setR123Zpcz(2); currentzpcz = 2;} else { rotate(0); state = 1;this->controller.setR123Zpcz(6); currentzpcz = 6;}
                break;
            case 3:
                if (event->button() == Qt::LeftButton)
                {rotate(90); state = 4;this->controller.setR123Zpcz(3); currentzpcz = 3;} else { rotate(30); state = 2;this->controller.setR123Zpcz(1); currentzpcz = 1;}
                break;
            case 4:
                if (event->button() == Qt::LeftButton)
                {rotate(120); state = 5;this->controller.setR123Zpcz(4); currentzpcz = 4;} else { rotate(60); state = 3;this->controller.setR123Zpcz(2); currentzpcz = 2;}
                break;
            case 5:
                if (event->button() == Qt::LeftButton)
                {rotate(150); state = 6;this->controller.setR123Zpcz(5); currentzpcz = 5;} else { rotate(90); state = 4;this->controller.setR123Zpcz(3); currentzpcz = 3;}
                break;
            case 6:
                if (event->button() == Qt::LeftButton)
                {rotate(0); state = 1;this->controller.setR123Zpcz(6); currentzpcz = 6;} else { rotate(120); state = 5;this->controller.setR123Zpcz(4); currentzpcz = 4;}
                break;
            }
            if (currentzpcz == 5)
                this->currentPoddiap = true;
            else if (currentzpcz == 6)
                this->currentPoddiap = false;
            else {
                this->currentPoddiap = this->r123_zpcz_switchers[currentzpcz - 1];
            }
            updateszkl();
            this->controller.setR123Zpcz(currentzpcz);
        } else if (event->x() > 267 && event->y() > 129
                   && event->x() < 370 && event->y() < 228 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            static int state = 1;
            auto rotate = [this](int angle) {
                static QPixmap pix(":/res/r123/r123_simp.png");
                QPixmap pixmap(pix);
                QMatrix rm;

                rm.rotate(angle);

                pixmap = pixmap.transformed(rm);

                ui->r123_simp->setPixmap(QPixmap(pixmap.transformed(rm)));
            };
            switch(state) {
            case 1:
                rotate(18); state = 2;
                break;
            case 2:
                rotate(0); state = 1;
                break;
            }
        } else if (event->x() > 853 && event->y() > 216
                   && event->x() < 872 && event->y() < 242 && this->r123_vechko_upped && r123_ziemLaunched) {
            this->keyReleaseEvent(nullptr);
            if (!this->r123_zpcz_switchers[0]) this->ui->r123_zpcz1_poddiap_sl->setPixmap(pixUpped);
            else this->ui->r123_zpcz1_poddiap_sl->setPixmap(pixDwned);
            this->r123_zpcz_switchers[0] = !this->r123_zpcz_switchers[0];
            if (this->currentzpcz == 1)
                this->currentPoddiap = this->r123_zpcz_switchers[0];
            this->updateszkl();
        } else if (event->x() > 887 && event->y() > 216
                   && event->x() < 913 && event->y() < 242 && this->r123_vechko_upped && r123_ziemLaunched) {
            this->keyReleaseEvent(nullptr);
            if (!this->r123_zpcz_switchers[1]) this->ui->r123_zpcz2_poddiap_sl->setPixmap(pixUpped);
            else this->ui->r123_zpcz2_poddiap_sl->setPixmap(pixDwned);
            this->r123_zpcz_switchers[1] = !this->r123_zpcz_switchers[1];
            if (this->currentzpcz == 2)
                this->currentPoddiap = this->r123_zpcz_switchers[1];
            this->updateszkl();
        } else if (event->x() > 922 && event->y() > 216
                   && event->x() < 945 && event->y() < 242 && this->r123_vechko_upped && r123_ziemLaunched) {
            this->keyReleaseEvent(nullptr);
            if (!this->r123_zpcz_switchers[2]) this->ui->r123_zpcz3_poddiap_sl->setPixmap(pixUpped);
            else this->ui->r123_zpcz3_poddiap_sl->setPixmap(pixDwned);
            this->r123_zpcz_switchers[2] = !this->r123_zpcz_switchers[2];
            if (this->currentzpcz == 3)
                this->currentPoddiap = this->r123_zpcz_switchers[2];
            this->updateszkl();
        } else if (event->x() > 953 && event->y() > 216
                   && event->x() < 972 && event->y() < 242 && this->r123_vechko_upped && r123_ziemLaunched) {
            this->keyReleaseEvent(nullptr);
            if (!this->r123_zpcz_switchers[3]) this->ui->r123_zpcz4_poddiap_sl->setPixmap(pixUpped);
            else this->ui->r123_zpcz4_poddiap_sl->setPixmap(pixDwned);
            this->r123_zpcz_switchers[3] = !this->r123_zpcz_switchers[3];
            if (this->currentzpcz == 4)
                this->currentPoddiap = this->r123_zpcz_switchers[3];
            this->updateszkl();
        }else if (event->x() > 815 && event->y() > 106
                   && event->x() < 998 && event->y() < 251 && r123_ziemLaunched)
        {
            this->keyReleaseEvent(nullptr);
            static bool launched = false;
            if (launched) {

                this->ui->r123_zpcz1_poddiap_sl->setPixmap(empty);
                this->ui->r123_zpcz2_poddiap_sl->setPixmap(empty);
                this->ui->r123_zpcz3_poddiap_sl->setPixmap(empty);
                this->ui->r123_zpcz4_poddiap_sl->setPixmap(empty);

                this->ui->r123_poddiap_switchers->setStyleSheet("");
                launched = false;

            } else {

                this->ui->r123_zpcz1_poddiap_sl->setPixmap(this->r123_zpcz_switchers[0] ? pixUpped : pixDwned);
                this->ui->r123_zpcz2_poddiap_sl->setPixmap(this->r123_zpcz_switchers[1] ? pixUpped : pixDwned);
                this->ui->r123_zpcz3_poddiap_sl->setPixmap(this->r123_zpcz_switchers[2] ? pixUpped : pixDwned);
                this->ui->r123_zpcz4_poddiap_sl->setPixmap(this->r123_zpcz_switchers[3] ? pixUpped : pixDwned);

                this->ui->r123_poddiap_switchers->setStyleSheet("background-image: url(:/res/r123/r123_poddiap_switchers.png);");
                launched = true;
            }
            this->r123_vechko_upped = launched;
        }

    }
}

void R123::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->vsua_nast_is_pressed == true)
    {
        static QPixmap pix(":/res/r123/vsua-nastr_fil.png");
        this->vsua_nast_is_pressed = false;
        this->ui->vsua_nastr_fil->setPixmap(pix);
    }
    if (this->vsua_clk_ind_is_pressed == true)
    {
        static QPixmap pix(":/res/r123/vsua-clk_ind_r.png");
        this->vsua_clk_ind_is_pressed = false;
        this->ui->vsua_clk_ind->setPixmap(pix);
    }
    if (this->r123_ton_call_pressed == true)
    {
        static QPixmap pix(":/res/r123/vsua-nastr_fil.png");
        this->r123_ton_call_pressed = false;
        this->ui->r123_ton_call->setPixmap(pix);
    }
}

int R123::countFrequency()
{
    if (!currentPoddiap)
        return freqUp;
    else return freqDown;
}

R123::R123(QString IP, bool is_server, AbstractNetworkController * controller) :
    QMainWindow(nullptr),
    ui(new Ui::r123),
    controller(this, IP, is_server, controller)
{
    ui->setupUi(this);
    this->vsua_nast_is_pressed = false;
    this->vsua_clk_ind_is_pressed = false;
    this->BP_active = false;
    this->r123_ustcz_can_move = true;
    this->r123_vechko_upped = false;
    this->currentFrequency = 20050;
    this->currentPoddiap = false;

    this->freqUp = 20050;
    this->freqDown = 35800;
    this->szkl_on = false;

    for (int i = 0; i < 4; i++)
        this->r123_zpcz_switchers[i] = false;

    this->r123_ziemLaunched = false;


    srand(time(nullptr));

    for (int i = 0; i < 6; i++)
        this->angle_to_be_setup[i] = rand() % 360;

    this->currentzpcz = 6;
    this->controller.setR123Zpcz(currentzpcz);

    angle_nastr = 0;

    updateOpacity();

    regim_raboty = 3;
    this->r123_canHandleKeys = true;
    power_on = false;
}

R123::~R123()
{
    delete ui;
}

void R123::rotate_r123_ustr(int angle)
{
    static QPixmap pix(":/res/r123/r123_ustr.png");
    QPixmap pixmap(pix);
    QMatrix rm;

    rm.rotate(angle);

    pixmap = pixmap.transformed(rm);

    ui->r123_ustr->setPixmap(QPixmap(pixmap.transformed(rm)));
}

void R123::rotate_vsua_ustr(int angle)
{
    static QPixmap pix(":/res/r123/vsua-ustr.png");
    QPixmap pixmap(pix);
    QMatrix rm;

    rm.rotate(angle);

    pixmap = pixmap.transformed(rm);

    ui->vsua_ustr->setPixmap(QPixmap(pixmap.transformed(rm)));
}

void R123::switch_BP_canTouch(bool state)
{
    this->BP_active = state;
}

void R123::setLineState(bool isActive)
{
    this->szkl_on = isActive;
    if (isActive)
    {
        this->ui->r123_line_off->setStyleSheet("");
    }
    else
    {
        this->ui->r123_line_off->setStyleSheet("background-image: url(:/res/r123/line/r123_line_black.png);");
    }
    updateszkl();
}

void R123::updateszkl()
{

    static QPixmap zpcz1_off(":/res/r123/numbers/r123_zpcz1.png");
    static QPixmap zpcz1_on(":/res/r123/numbers/r123_zpcz1_on.png");

    static QPixmap zpcz2_off(":/res/r123/numbers/r123_zpcz2.png");
    static QPixmap zpcz2_on(":/res/r123/numbers/r123_zpcz2_on.png");

    static QPixmap zpcz3_off(":/res/r123/numbers/r123_zpcz3.png");
    static QPixmap zpcz3_on(":/res/r123/numbers/r123_zpcz3_on.png");

    static QPixmap zpcz4_off(":/res/r123/numbers/r123_zpcz4.png");
    static QPixmap zpcz4_on(":/res/r123/numbers/r123_zpcz4_on.png");

    static QPixmap poddiap1_off(":/res/r123/numbers/r123_poddiap1.png");
    static QPixmap poddiap1_on(":/res/r123/numbers/r123_poddiap1_on.png");

    static QPixmap poddiap2_off(":/res/r123/numbers/r123_poddiap2.png");
    static QPixmap poddiap2_on(":/res/r123/numbers/r123_poddiap2_on.png");
    static auto off_all = [this](){
        this->ui->r123_zpcz1->setPixmap(zpcz1_off);
        this->ui->r123_zpcz2->setPixmap(zpcz2_off);
        this->ui->r123_zpcz3->setPixmap(zpcz3_off);
        this->ui->r123_zpcz4->setPixmap(zpcz4_off);
        this->ui->r123_poddiap1->setPixmap(poddiap1_off);
        this->ui->r123_poddiap2->setPixmap(poddiap2_off);
    };
    if (szkl_on)
    {
        switch(currentzpcz) {
        case 1:
            off_all();
            this->ui->r123_zpcz1->setPixmap(zpcz1_on);
            break;
        case 2:
            off_all();
            this->ui->r123_zpcz2->setPixmap(zpcz2_on);
            break;
        case 3:
            off_all();
            this->ui->r123_zpcz3->setPixmap(zpcz3_on);
            break;
        case 4:
            off_all();
            this->ui->r123_zpcz4->setPixmap(zpcz4_on);
            break;
        default:
            off_all();
        }
        if (!currentPoddiap)
        {
            this->ui->r123_poddiap1->setPixmap(poddiap1_on);
            this->ui->r123_poddiap2->setPixmap(poddiap2_off);
        }
        else
        {
            this->ui->r123_poddiap1->setPixmap(poddiap1_off);
            this->ui->r123_poddiap2->setPixmap(poddiap2_on);
        }
    }
    else {
        off_all();
    }
}

void R123::updateOpacity()
{
    int rotate_delta = abs(getAngleToBeSetup() - angle_nastr);

    if (regim_raboty == 3 && this->space_is_pressed && power_on)
    {

        setOpacity(0.0);
        rotate_r123_ustr(33);
        return;
    }


    if (!this->power_on || !this->space_is_pressed)
    {

        setOpacity(0.);
        rotate_r123_ustr(0);
        this->controller.setNastr(false);
    }
    else if (rotate_delta < 12)
    {
        setOpacity(1.0f);
        rotate_r123_ustr(33);
        this->controller.setNastr(true, countFrequency());
    } else if (rotate_delta < 40)
    {
        setOpacity(1.0f - float(rotate_delta - 12) / 28);
        rotate_r123_ustr(int((1.0f - float(rotate_delta - 12) / 28) * 33));
        this->controller.setNastr(false);
    }
    else
    {
        rotate_r123_ustr(0);
        setOpacity(0.);
        this->controller.setNastr(false);
    }
}

void R123::keyPressEvent(QKeyEvent *ev)
{
    if (!ev->isAutoRepeat() && this->r123_canHandleKeys && this->r123_ziemLaunched)
    {
        if (regim_raboty == 1 || regim_raboty == 2)
        {
            controller.keyPressHandler(ev);
            if (ev->key() == Qt::Key_Space)
            {
                this->space_is_pressed = true;
                updateOpacity();
            }
        }
        else if (regim_raboty == 3)
        {
            if (ev->key() == Qt::Key_Space)
            {
                this->rotate_vsua_ustr(33);
                this->space_is_pressed = true;
                updateOpacity();
            }
        }
    }
}

// 33 degrees max r123_ustr;

void R123::keyReleaseEvent(QKeyEvent *ev)
{
    if (!ev || (ev->key() == Qt::Key_Space && !ev->isAutoRepeat()))
    {
        controller.keyReleaseHandler(ev);
        this->space_is_pressed = false;
        updateOpacity();
    }
}


void R123::setFrequency(int frequency)
{

}

int R123::getAngleToBeSetup()
{
    return this->angle_to_be_setup[currentzpcz - 1];
}

void R123::setOpacity(float opacity)
{
    opacity *= 255;
    QImage image(":/res/r123/r123_setup_diod.png");
    QPainter p;
    p.begin(&image);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(image.rect(), QColor(0, 0, 0, int(opacity)));
    p.end();

    this->ui->r123_diod_setup->setPixmap(QPixmap::fromImage(image));
}

void R123::setPoddiap(bool poddiap)
{

}

void R123::setPowerState(bool power)
{
    this->power_on = power;
    updateOpacity();
}

void R123::r123_setHandleKeys(bool canHandle)
{
    this->r123_canHandleKeys = canHandle;
}



