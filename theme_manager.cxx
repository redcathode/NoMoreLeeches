#include "theme_manager.h"
// All credit goes to https://github.com/fltk-rs/fltk-theme for these


void theme_manager::setColor(int one, int two, int three, int four) {
  Fl::set_color(Fl_Color(one), two, three, four);
}
void theme_manager::loadColorScheme(int themeIndex) { // 0=dark, 1=black, 2=gray, 3=tan, 4=shake
   switch(themeIndex) {
       case 0:
           loadDarkColorScheme();
           break;
       case 1:
           loadBlackColorScheme();
           break;
       case 2:
           loadGrayColorScheme();
           break;
       case 3:
           loadTanColorScheme();
           break;
       case 4:
           loadShakeColorScheme();
           break;
   }
   Fl::reload_scheme(); // if plastic/gtk+/etc, update with new colors
}

void theme_manager::loadShakeColorScheme() {
    setColor(0, 180, 180, 180);
    setColor(1, 162, 48, 48);
    setColor(2, 90, 180, 0);
    setColor(3, 255, 255, 0);
    setColor(4, 88, 120, 152);
    setColor(5, 180, 0, 180);
    setColor(6, 0, 180, 180);
    setColor(7, 80, 80, 80);
    setColor(8, 100, 100, 100);
    setColor(9, 119, 71, 71);
    setColor(10, 80, 130, 80);
    setColor(11, 150, 150, 90);
    setColor(12, 110, 123, 145);
    setColor(13, 150, 90, 150);
    setColor(14, 90, 150, 150);
    setColor(15, 200, 50, 50);
    setColor(32, 3, 3, 3);
    setColor(33, 5, 5, 5);
    setColor(34, 8, 8, 8);
    setColor(35, 10, 10, 10);
    setColor(36, 14, 14, 14);
    setColor(37, 16, 16, 16);
    setColor(38, 21, 21, 21);
    setColor(39, 26, 26, 26);
    setColor(40, 32, 32, 32);
    setColor(41, 37, 37, 37);
    setColor(42, 42, 42, 42);
    setColor(43, 48, 48, 48);
    setColor(44, 53, 53, 53);
    setColor(45, 58, 58, 58);
    setColor(46, 64, 64, 64);
    setColor(47, 69, 69, 69);
    setColor(48, 74, 74, 74);
    setColor(49, 80, 80, 80);
    setColor(50, 85, 85, 85);
    setColor(51, 90, 90, 90);
    setColor(52, 96, 96, 96);
    setColor(53, 101, 101, 101);
    setColor(54, 106, 106, 106);
    setColor(55, 110, 110, 110);
    setColor(56, 150, 150, 150);
    setColor(59, 80, 150, 80);
    setColor(63, 0, 180, 0);
    setColor(71, 0, 180, 0);
    setColor(88, 180, 0, 0);
    setColor(90, 180, 80, 40);
    setColor(91, 180, 120, 0);
    setColor(94, 150, 110, 20);
    setColor(95, 120, 120, 36);
    setColor(124, 107, 92, 57);
    setColor(254, 60, 70, 70);
    setColor(255, 50, 50, 50);
}

void theme_manager::loadTanColorScheme() {
    setColor(0, 0, 0, 0);
    setColor(1, 255, 0, 0);
    setColor(2, 0, 255, 0);
    setColor(3, 255, 255, 0);
    setColor(4, 0, 0, 255);
    setColor(5, 255, 0, 255);
    setColor(6, 0, 255, 255);
    setColor(7, 255, 255, 255);
    setColor(8, 85, 85, 85);
    setColor(9, 198, 113, 113);
    setColor(10, 113, 198, 113);
    setColor(11, 142, 142, 56);
    setColor(12, 113, 113, 198);
    setColor(13, 200, 150, 200);
    setColor(14, 56, 142, 142);
    setColor(15, 0, 0, 128);
    setColor(32, 0, 0, 0);
    setColor(33, 114, 100, 46);
    setColor(34, 120, 107, 56);
    setColor(35, 127, 114, 65);
    setColor(36, 133, 121, 75);
    setColor(37, 140, 128, 84);
    setColor(38, 146, 135, 94);
    setColor(39, 152, 142, 103);
    setColor(40, 159, 149, 113);
    setColor(41, 165, 156, 122);
    setColor(42, 172, 163, 132);
    setColor(43, 178, 170, 141);
    setColor(44, 185, 178, 151);
    setColor(45, 191, 185, 160);
    setColor(46, 197, 192, 170);
    setColor(47, 204, 199, 179);
    setColor(48, 210, 206, 189);
    setColor(49, 217, 213, 198);
    setColor(50, 223, 220, 208);
    setColor(51, 229, 227, 217);
    setColor(52, 236, 234, 227);
    setColor(53, 242, 241, 236);
    setColor(54, 249, 248, 246);
    setColor(55, 255, 255, 255);
    setColor(56, 0, 0, 0);
    setColor(57, 0, 36, 0);
    setColor(58, 0, 73, 0);
    setColor(59, 0, 109, 0);
    setColor(60, 0, 146, 0);
    setColor(61, 0, 182, 0);
    setColor(62, 0, 219, 0);
    setColor(63, 0, 255, 0);
    setColor(64, 64, 0, 0);
    setColor(65, 64, 36, 0);
    setColor(66, 64, 73, 0);
    setColor(67, 64, 109, 0);
    setColor(68, 64, 146, 0);
    setColor(69, 64, 182, 0);
    setColor(70, 64, 219, 0);
    setColor(71, 64, 255, 0);
    setColor(72, 128, 0, 0);
    setColor(73, 128, 36, 0);
    setColor(74, 128, 73, 0);
    setColor(75, 128, 109, 0);
    setColor(76, 128, 146, 0);
    setColor(77, 128, 182, 0);
    setColor(78, 128, 219, 0);
    setColor(79, 128, 255, 0);
    setColor(80, 191, 0, 0);
    setColor(81, 191, 36, 0);
    setColor(82, 191, 73, 0);
    setColor(83, 191, 109, 0);
    setColor(84, 191, 146, 0);
    setColor(85, 191, 182, 0);
    setColor(86, 191, 219, 0);
    setColor(87, 191, 255, 0);
    setColor(88, 255, 0, 0);
    setColor(89, 255, 36, 0);
    setColor(90, 255, 73, 0);
    setColor(91, 255, 109, 0);
    setColor(92, 255, 146, 0);
    setColor(93, 255, 182, 0);
    setColor(94, 255, 219, 0);
    setColor(95, 255, 255, 0);
    setColor(96, 0, 0, 64);
    setColor(97, 0, 36, 64);
    setColor(98, 0, 73, 64);
    setColor(99, 0, 109, 64);
    setColor(100, 0, 146, 64);
    setColor(101, 0, 182, 64);
    setColor(102, 0, 219, 64);
    setColor(103, 0, 255, 64);
    setColor(104, 64, 0, 64);
    setColor(105, 64, 36, 64);
    setColor(106, 64, 73, 64);
    setColor(107, 64, 109, 64);
    setColor(108, 64, 146, 64);
    setColor(109, 64, 182, 64);
    setColor(110, 64, 219, 64);
    setColor(111, 64, 255, 64);
    setColor(112, 128, 0, 64);
    setColor(113, 128, 36, 64);
    setColor(114, 128, 73, 64);
    setColor(115, 128, 109, 64);
    setColor(116, 128, 146, 64);
    setColor(117, 128, 182, 64);
    setColor(118, 128, 219, 64);
    setColor(119, 128, 255, 64);
    setColor(120, 191, 0, 64);
    setColor(121, 191, 36, 64);
    setColor(122, 191, 73, 64);
    setColor(123, 191, 109, 64);
    setColor(124, 191, 146, 64);
    setColor(125, 191, 182, 64);
    setColor(126, 191, 219, 64);
    setColor(127, 191, 255, 64);
    setColor(128, 255, 0, 64);
    setColor(129, 255, 36, 64);
    setColor(130, 255, 73, 64);
    setColor(131, 255, 109, 64);
    setColor(132, 255, 146, 64);
    setColor(133, 255, 182, 64);
    setColor(134, 255, 219, 64);
    setColor(135, 255, 255, 64);
    setColor(136, 0, 0, 128);
    setColor(137, 0, 36, 128);
    setColor(138, 0, 73, 128);
    setColor(139, 0, 109, 128);
    setColor(140, 0, 146, 128);
    setColor(141, 0, 182, 128);
    setColor(142, 0, 219, 128);
    setColor(143, 0, 255, 128);
    setColor(144, 64, 0, 128);
    setColor(145, 64, 36, 128);
    setColor(146, 64, 73, 128);
    setColor(147, 64, 109, 128);
    setColor(148, 64, 146, 128);
    setColor(149, 64, 182, 128);
    setColor(150, 64, 219, 128);
    setColor(151, 64, 255, 128);
    setColor(152, 128, 0, 128);
    setColor(153, 128, 36, 128);
    setColor(154, 128, 73, 128);
    setColor(155, 128, 109, 128);
    setColor(156, 128, 146, 128);
    setColor(157, 128, 182, 128);
    setColor(158, 128, 219, 128);
    setColor(159, 128, 255, 128);
    setColor(160, 191, 0, 128);
    setColor(161, 191, 36, 128);
    setColor(162, 191, 73, 128);
    setColor(163, 191, 109, 128);
    setColor(164, 191, 146, 128);
    setColor(165, 191, 182, 128);
    setColor(166, 191, 219, 128);
    setColor(167, 191, 255, 128);
    setColor(168, 255, 0, 128);
    setColor(169, 255, 36, 128);
    setColor(170, 255, 73, 128);
    setColor(171, 255, 109, 128);
    setColor(172, 255, 146, 128);
    setColor(173, 255, 182, 128);
    setColor(174, 255, 219, 128);
    setColor(175, 255, 255, 128);
    setColor(176, 0, 0, 191);
    setColor(177, 0, 36, 191);
    setColor(178, 0, 73, 191);
    setColor(179, 0, 109, 191);
    setColor(180, 0, 146, 191);
    setColor(181, 0, 182, 191);
    setColor(182, 0, 219, 191);
    setColor(183, 0, 255, 191);
    setColor(184, 64, 0, 191);
    setColor(185, 64, 36, 191);
    setColor(186, 64, 73, 191);
    setColor(187, 64, 109, 191);
    setColor(188, 64, 146, 191);
    setColor(189, 64, 182, 191);
    setColor(190, 64, 219, 191);
    setColor(191, 64, 255, 191);
    setColor(192, 128, 0, 191);
    setColor(193, 128, 36, 191);
    setColor(194, 128, 73, 191);
    setColor(195, 128, 109, 191);
    setColor(196, 128, 146, 191);
    setColor(197, 128, 182, 191);
    setColor(198, 128, 219, 191);
    setColor(199, 128, 255, 191);
    setColor(200, 191, 0, 191);
    setColor(201, 191, 36, 191);
    setColor(202, 191, 73, 191);
    setColor(203, 191, 109, 191);
    setColor(204, 191, 146, 191);
    setColor(205, 191, 182, 191);
    setColor(206, 191, 219, 191);
    setColor(207, 191, 255, 191);
    setColor(208, 255, 0, 191);
    setColor(209, 255, 36, 191);
    setColor(210, 255, 73, 191);
    setColor(211, 255, 109, 191);
    setColor(212, 255, 146, 191);
    setColor(213, 255, 182, 191);
    setColor(214, 255, 219, 191);
    setColor(215, 255, 255, 191);
    setColor(216, 0, 0, 255);
    setColor(217, 0, 36, 255);
    setColor(218, 0, 73, 255);
    setColor(219, 0, 109, 255);
    setColor(220, 0, 146, 255);
    setColor(221, 0, 182, 255);
    setColor(222, 0, 219, 255);
    setColor(223, 0, 255, 255);
    setColor(224, 64, 0, 255);
    setColor(225, 64, 36, 255);
    setColor(226, 64, 73, 255);
    setColor(227, 64, 109, 255);
    setColor(228, 64, 146, 255);
    setColor(229, 64, 182, 255);
    setColor(230, 64, 219, 255);
    setColor(231, 64, 255, 255);
    setColor(232, 128, 0, 255);
    setColor(233, 128, 36, 255);
    setColor(234, 128, 73, 255);
    setColor(235, 128, 109, 255);
    setColor(236, 128, 146, 255);
    setColor(237, 128, 182, 255);
    setColor(238, 128, 219, 255);
    setColor(239, 128, 255, 255);
    setColor(240, 191, 0, 255);
    setColor(241, 191, 36, 255);
    setColor(242, 191, 73, 255);
    setColor(243, 191, 109, 255);
    setColor(244, 191, 146, 255);
    setColor(245, 191, 182, 255);
    setColor(246, 191, 219, 255);
    setColor(247, 191, 255, 255);
    setColor(248, 255, 0, 255);
    setColor(249, 255, 36, 255);
    setColor(250, 255, 73, 255);
    setColor(251, 255, 109, 255);
    setColor(252, 255, 146, 255);
    setColor(253, 255, 182, 255);
    setColor(254, 255, 219, 255);
    setColor(255, 255, 255, 255);
    setColor(254, 220, 240, 240);
}

void theme_manager::loadGrayColorScheme() {
    setColor(0, 0, 0, 0);
    setColor(1, 255, 0, 0);
    setColor(2, 0, 255, 0);
    setColor(3, 255, 255, 0);
    setColor(4, 0, 0, 255);
    setColor(5, 255, 0, 255);
    setColor(6, 0, 255, 255);
    setColor(7, 255, 255, 255);
    setColor(8, 85, 85, 85);
    setColor(9, 198, 113, 113);
    setColor(10, 113, 198, 113);
    setColor(11, 142, 142, 56);
    setColor(12, 113, 113, 198);
    setColor(13, 142, 56, 142);
    setColor(14, 56, 142, 142);
    setColor(15, 0, 0, 128);
    setColor(32, 0, 0, 0);
    setColor(33, 13, 13, 13);
    setColor(34, 26, 26, 26);
    setColor(35, 38, 38, 38);
    setColor(36, 49, 49, 49);
    setColor(37, 61, 61, 61);
    setColor(38, 72, 72, 72);
    setColor(39, 85, 85, 85);
    setColor(40, 95, 95, 95);
    setColor(41, 106, 106, 106);
    setColor(42, 117, 117, 117);
    setColor(43, 128, 128, 128);
    setColor(44, 138, 138, 138);
    setColor(45, 149, 149, 149);
    setColor(46, 160, 160, 160);
    setColor(47, 170, 170, 170);
    setColor(48, 181, 181, 181);
    setColor(49, 192, 192, 192);
    setColor(50, 203, 203, 203);
    setColor(51, 213, 213, 213);
    setColor(52, 224, 224, 224);
    setColor(53, 234, 234, 234);
    setColor(54, 245, 245, 245);
    setColor(55, 255, 255, 255);
    setColor(56, 0, 0, 0);
    setColor(57, 0, 36, 0);
    setColor(58, 0, 73, 0);
    setColor(59, 0, 109, 0);
    setColor(60, 0, 146, 0);
    setColor(61, 0, 182, 0);
    setColor(62, 0, 219, 0);
    setColor(63, 0, 255, 0);
    setColor(64, 64, 0, 0);
    setColor(65, 64, 36, 0);
    setColor(66, 64, 73, 0);
    setColor(67, 64, 109, 0);
    setColor(68, 64, 146, 0);
    setColor(69, 64, 182, 0);
    setColor(70, 64, 219, 0);
    setColor(71, 64, 255, 0);
    setColor(72, 128, 0, 0);
    setColor(73, 128, 36, 0);
    setColor(74, 128, 73, 0);
    setColor(75, 128, 109, 0);
    setColor(76, 128, 146, 0);
    setColor(77, 128, 182, 0);
    setColor(78, 128, 219, 0);
    setColor(79, 128, 255, 0);
    setColor(80, 191, 0, 0);
    setColor(81, 191, 36, 0);
    setColor(82, 191, 73, 0);
    setColor(83, 191, 109, 0);
    setColor(84, 191, 146, 0);
    setColor(85, 191, 182, 0);
    setColor(86, 191, 219, 0);
    setColor(87, 191, 255, 0);
    setColor(88, 255, 0, 0);
    setColor(89, 255, 36, 0);
    setColor(90, 255, 73, 0);
    setColor(91, 255, 109, 0);
    setColor(92, 255, 146, 0);
    setColor(93, 255, 182, 0);
    setColor(94, 255, 219, 0);
    setColor(95, 255, 255, 0);
    setColor(96, 0, 0, 64);
    setColor(97, 0, 36, 64);
    setColor(98, 0, 73, 64);
    setColor(99, 0, 109, 64);
    setColor(100, 0, 146, 64);
    setColor(101, 0, 182, 64);
    setColor(102, 0, 219, 64);
    setColor(103, 0, 255, 64);
    setColor(104, 64, 0, 64);
    setColor(105, 64, 36, 64);
    setColor(106, 64, 73, 64);
    setColor(107, 64, 109, 64);
    setColor(108, 64, 146, 64);
    setColor(109, 64, 182, 64);
    setColor(110, 64, 219, 64);
    setColor(111, 64, 255, 64);
    setColor(112, 128, 0, 64);
    setColor(113, 128, 36, 64);
    setColor(114, 128, 73, 64);
    setColor(115, 128, 109, 64);
    setColor(116, 128, 146, 64);
    setColor(117, 128, 182, 64);
    setColor(118, 128, 219, 64);
    setColor(119, 128, 255, 64);
    setColor(120, 191, 0, 64);
    setColor(121, 191, 36, 64);
    setColor(122, 191, 73, 64);
    setColor(123, 191, 109, 64);
    setColor(124, 191, 146, 64);
    setColor(125, 191, 182, 64);
    setColor(126, 191, 219, 64);
    setColor(127, 191, 255, 64);
    setColor(128, 255, 0, 64);
    setColor(129, 255, 36, 64);
    setColor(130, 255, 73, 64);
    setColor(131, 255, 109, 64);
    setColor(132, 255, 146, 64);
    setColor(133, 255, 182, 64);
    setColor(134, 255, 219, 64);
    setColor(135, 255, 255, 64);
    setColor(136, 0, 0, 128);
    setColor(137, 0, 36, 128);
    setColor(138, 0, 73, 128);
    setColor(139, 0, 109, 128);
    setColor(140, 0, 146, 128);
    setColor(141, 0, 182, 128);
    setColor(142, 0, 219, 128);
    setColor(143, 0, 255, 128);
    setColor(144, 64, 0, 128);
    setColor(145, 64, 36, 128);
    setColor(146, 64, 73, 128);
    setColor(147, 64, 109, 128);
    setColor(148, 64, 146, 128);
    setColor(149, 64, 182, 128);
    setColor(150, 64, 219, 128);
    setColor(151, 64, 255, 128);
    setColor(152, 128, 0, 128);
    setColor(153, 128, 36, 128);
    setColor(154, 128, 73, 128);
    setColor(155, 128, 109, 128);
    setColor(156, 128, 146, 128);
    setColor(157, 128, 182, 128);
    setColor(158, 128, 219, 128);
    setColor(159, 128, 255, 128);
    setColor(160, 191, 0, 128);
    setColor(161, 191, 36, 128);
    setColor(162, 191, 73, 128);
    setColor(163, 191, 109, 128);
    setColor(164, 191, 146, 128);
    setColor(165, 191, 182, 128);
    setColor(166, 191, 219, 128);
    setColor(167, 191, 255, 128);
    setColor(168, 255, 0, 128);
    setColor(169, 255, 36, 128);
    setColor(170, 255, 73, 128);
    setColor(171, 255, 109, 128);
    setColor(172, 255, 146, 128);
    setColor(173, 255, 182, 128);
    setColor(174, 255, 219, 128);
    setColor(175, 255, 255, 128);
    setColor(176, 0, 0, 191);
    setColor(177, 0, 36, 191);
    setColor(178, 0, 73, 191);
    setColor(179, 0, 109, 191);
    setColor(180, 0, 146, 191);
    setColor(181, 0, 182, 191);
    setColor(182, 0, 219, 191);
    setColor(183, 0, 255, 191);
    setColor(184, 64, 0, 191);
    setColor(185, 64, 36, 191);
    setColor(186, 64, 73, 191);
    setColor(187, 64, 109, 191);
    setColor(188, 64, 146, 191);
    setColor(189, 64, 182, 191);
    setColor(190, 64, 219, 191);
    setColor(191, 64, 255, 191);
    setColor(192, 128, 0, 191);
    setColor(193, 128, 36, 191);
    setColor(194, 128, 73, 191);
    setColor(195, 128, 109, 191);
    setColor(196, 128, 146, 191);
    setColor(197, 128, 182, 191);
    setColor(198, 128, 219, 191);
    setColor(199, 128, 255, 191);
    setColor(200, 191, 0, 191);
    setColor(201, 191, 36, 191);
    setColor(202, 191, 73, 191);
    setColor(203, 191, 109, 191);
    setColor(204, 191, 146, 191);
    setColor(205, 191, 182, 191);
    setColor(206, 191, 219, 191);
    setColor(207, 191, 255, 191);
    setColor(208, 255, 0, 191);
    setColor(209, 255, 36, 191);
    setColor(210, 255, 73, 191);
    setColor(211, 255, 109, 191);
    setColor(212, 255, 146, 191);
    setColor(213, 255, 182, 191);
    setColor(214, 255, 219, 191);
    setColor(215, 255, 255, 191);
    setColor(216, 0, 0, 255);
    setColor(217, 0, 36, 255);
    setColor(218, 0, 73, 255);
    setColor(219, 0, 109, 255);
    setColor(220, 0, 146, 255);
    setColor(221, 0, 182, 255);
    setColor(222, 0, 219, 255);
    setColor(223, 0, 255, 255);
    setColor(224, 64, 0, 255);
    setColor(225, 64, 36, 255);
    setColor(226, 64, 73, 255);
    setColor(227, 64, 109, 255);
    setColor(228, 64, 146, 255);
    setColor(229, 64, 182, 255);
    setColor(230, 64, 219, 255);
    setColor(231, 64, 255, 255);
    setColor(232, 128, 0, 255);
    setColor(233, 128, 36, 255);
    setColor(234, 128, 73, 255);
    setColor(235, 128, 109, 255);
    setColor(236, 128, 146, 255);
    setColor(237, 128, 182, 255);
    setColor(238, 128, 219, 255);
    setColor(239, 128, 255, 255);
    setColor(240, 191, 0, 255);
    setColor(241, 191, 36, 255);
    setColor(242, 191, 73, 255);
    setColor(243, 191, 109, 255);
    setColor(244, 191, 146, 255);
    setColor(245, 191, 182, 255);
    setColor(246, 191, 219, 255);
    setColor(247, 191, 255, 255);
    setColor(248, 255, 0, 255);
    setColor(249, 255, 36, 255);
    setColor(250, 255, 73, 255);
    setColor(251, 255, 109, 255);
    setColor(252, 255, 146, 255);
    setColor(253, 255, 182, 255);
    setColor(254, 255, 219, 255);
    setColor(255, 255, 255, 255);
    setColor(254, 220, 240, 240);
}
void theme_manager::loadDarkColorScheme() {
    setColor(0, 200, 200, 200);
    setColor(1, 150, 30, 30);
    setColor(2, 0, 180, 0);
    setColor(3, 180, 180, 0);
    setColor(4, 0, 0, 180);
    setColor(5, 180, 0, 180);
    setColor(6, 0, 180, 180);
    setColor(7, 50, 50, 50);
    setColor(8, 100, 100, 100);
    setColor(9, 150, 90, 90);
    setColor(10, 90, 150, 90);
    setColor(11, 150, 150, 90);
    setColor(12, 90, 90, 150);
    setColor(13, 150, 90, 150);
    setColor(14, 90, 150, 150);
    setColor(15, 150, 150, 150);
    setColor(32, 5, 5, 5);
    setColor(33, 10, 10, 10);
    setColor(34, 16, 16, 16);
    setColor(35, 21, 21, 21);
    setColor(36, 26, 26, 26);
    setColor(37, 32, 32, 32);
    setColor(38, 37, 37, 37);
    setColor(39, 42, 42, 42);
    setColor(40, 48, 48, 48);
    setColor(41, 53, 53, 53);
    setColor(42, 58, 58, 58);
    setColor(43, 64, 64, 64);
    setColor(44, 69, 69, 69);
    setColor(45, 74, 74, 74);
    setColor(46, 80, 80, 80);
    setColor(47, 85, 85, 85);
    setColor(48, 90, 90, 90);
    setColor(49, 96, 96, 96);
    setColor(50, 101, 101, 101);
    setColor(51, 106, 106, 106);
    setColor(52, 112, 112, 112);
    setColor(53, 117, 117, 117);
    setColor(54, 122, 122, 122);
    setColor(55, 180, 180, 180);
    setColor(56, 0, 0, 0);
    setColor(59, 30, 140, 30);
    setColor(63, 0, 180, 0);
    setColor(71, 0, 180, 0);
    setColor(88, 180, 0, 0);
    setColor(90, 200, 72, 60);
    setColor(91, 180, 120, 0);
    setColor(95, 180, 180, 0);
    setColor(124, 191, 145, 63);
    setColor(94, 150, 100, 30);
    setColor(254, 60, 66, 66);
    setColor(255, 50, 50, 50);
}

void theme_manager::loadBlackColorScheme() {
    setColor(0, 140, 140, 140);
    setColor(1, 120, 0, 0);
    setColor(2, 0, 120, 0);
    setColor(3, 120, 120, 0);
    setColor(4, 0, 0, 120);
    setColor(5, 120, 0, 120);
    setColor(6, 0, 120, 120);
    setColor(7, 0, 0, 0);
    setColor(8, 100, 100, 100);
    setColor(9, 100, 0, 0);
    setColor(10, 0, 100, 0);
    setColor(11, 100, 100, 0);
    setColor(12, 0, 0, 100);
    setColor(13, 100, 0, 100);
    setColor(14, 0, 100, 100);
    setColor(15, 50, 50, 50);
    setColor(32, 180, 180, 180);
    setColor(33, 122, 122, 122);
    setColor(34, 117, 117, 117);
    setColor(35, 112, 112, 112);
    setColor(36, 106, 106, 106);
    setColor(37, 101, 101, 101);
    setColor(38, 96, 96, 96);
    setColor(39, 90, 90, 90);
    setColor(40, 85, 85, 85);
    setColor(41, 80, 80, 80);
    setColor(42, 74, 74, 74);
    setColor(43, 69, 69, 69);
    setColor(44, 64, 64, 64);
    setColor(45, 58, 58, 58);
    setColor(46, 53, 53, 53);
    setColor(47, 48, 48, 48);
    setColor(48, 42, 42, 42);
    setColor(49, 37, 37, 37);
    setColor(50, 32, 32, 32);
    setColor(51, 26, 26, 26);
    setColor(52, 21, 21, 21);
    setColor(53, 16, 16, 16);
    setColor(54, 10, 10, 10);
    setColor(55, 0, 0, 0);
    setColor(56, 0, 0, 0);
    setColor(57, 0, 16, 0);
    setColor(58, 0, 31, 0);
    setColor(59, 0, 47, 0);
    setColor(60, 0, 63, 0);
    setColor(61, 0, 79, 0);
    setColor(62, 0, 94, 0);
    setColor(63, 0, 110, 0);
    setColor(64, 28, 0, 0);
    setColor(65, 28, 16, 0);
    setColor(66, 28, 31, 0);
    setColor(67, 28, 47, 0);
    setColor(68, 28, 63, 0);
    setColor(69, 28, 79, 0);
    setColor(70, 28, 94, 0);
    setColor(71, 28, 110, 0);
    setColor(72, 55, 0, 0);
    setColor(73, 55, 16, 0);
    setColor(74, 55, 31, 0);
    setColor(75, 55, 47, 0);
    setColor(76, 55, 63, 0);
    setColor(77, 55, 79, 0);
    setColor(78, 55, 94, 0);
    setColor(79, 55, 110, 0);
    setColor(80, 83, 0, 0);
    setColor(81, 83, 16, 0);
    setColor(82, 83, 31, 0);
    setColor(83, 83, 47, 0);
    setColor(84, 83, 63, 0);
    setColor(85, 83, 79, 0);
    setColor(86, 83, 94, 0);
    setColor(87, 83, 110, 0);
    setColor(88, 110, 0, 0);
    setColor(89, 110, 16, 0);
    setColor(90, 110, 31, 0);
    setColor(91, 110, 47, 0);
    setColor(92, 110, 63, 0);
    setColor(93, 110, 79, 0);
    setColor(94, 110, 94, 0);
    setColor(95, 110, 110, 0);
    setColor(96, 0, 0, 28);
    setColor(97, 0, 16, 28);
    setColor(98, 0, 31, 28);
    setColor(99, 0, 47, 28);
    setColor(100, 0, 63, 28);
    setColor(101, 0, 79, 28);
    setColor(102, 0, 94, 28);
    setColor(103, 0, 110, 28);
    setColor(104, 28, 0, 28);
    setColor(105, 28, 16, 28);
    setColor(106, 28, 31, 28);
    setColor(107, 28, 47, 28);
    setColor(108, 28, 63, 28);
    setColor(109, 28, 79, 28);
    setColor(110, 28, 94, 28);
    setColor(111, 28, 110, 28);
    setColor(112, 55, 0, 28);
    setColor(113, 55, 16, 28);
    setColor(114, 55, 31, 28);
    setColor(115, 55, 47, 28);
    setColor(116, 55, 63, 28);
    setColor(117, 55, 79, 28);
    setColor(118, 55, 94, 28);
    setColor(119, 55, 110, 28);
    setColor(120, 83, 0, 28);
    setColor(121, 83, 16, 28);
    setColor(122, 83, 31, 28);
    setColor(123, 83, 47, 28);
    setColor(124, 83, 63, 28);
    setColor(125, 83, 79, 28);
    setColor(126, 83, 94, 28);
    setColor(127, 83, 110, 28);
    setColor(128, 110, 0, 28);
    setColor(129, 110, 16, 28);
    setColor(130, 110, 31, 28);
    setColor(131, 110, 47, 28);
    setColor(132, 110, 63, 28);
    setColor(133, 110, 79, 28);
    setColor(134, 110, 94, 28);
    setColor(135, 110, 110, 28);
    setColor(136, 0, 0, 55);
    setColor(137, 0, 16, 55);
    setColor(138, 0, 31, 55);
    setColor(139, 0, 47, 55);
    setColor(140, 0, 63, 55);
    setColor(141, 0, 79, 55);
    setColor(142, 0, 94, 55);
    setColor(143, 0, 110, 55);
    setColor(144, 28, 0, 55);
    setColor(145, 28, 16, 55);
    setColor(146, 28, 31, 55);
    setColor(147, 28, 47, 55);
    setColor(148, 28, 63, 55);
    setColor(149, 28, 79, 55);
    setColor(150, 28, 94, 55);
    setColor(151, 28, 110, 55);
    setColor(152, 55, 0, 55);
    setColor(153, 55, 16, 55);
    setColor(154, 55, 31, 55);
    setColor(155, 55, 47, 55);
    setColor(156, 55, 63, 55);
    setColor(157, 55, 79, 55);
    setColor(158, 55, 94, 55);
    setColor(159, 55, 110, 55);
    setColor(160, 83, 0, 55);
    setColor(161, 83, 16, 55);
    setColor(162, 83, 31, 55);
    setColor(163, 83, 47, 55);
    setColor(164, 83, 63, 55);
    setColor(165, 83, 79, 55);
    setColor(166, 83, 94, 55);
    setColor(167, 83, 110, 55);
    setColor(168, 110, 0, 55);
    setColor(169, 110, 16, 55);
    setColor(170, 110, 31, 55);
    setColor(171, 110, 47, 55);
    setColor(172, 110, 63, 55);
    setColor(173, 110, 79, 55);
    setColor(174, 110, 94, 55);
    setColor(175, 110, 110, 55);
    setColor(176, 0, 0, 83);
    setColor(177, 0, 16, 83);
    setColor(178, 0, 31, 83);
    setColor(179, 0, 47, 83);
    setColor(180, 0, 63, 83);
    setColor(181, 0, 79, 83);
    setColor(182, 0, 94, 83);
    setColor(183, 0, 110, 83);
    setColor(184, 28, 0, 83);
    setColor(185, 28, 16, 83);
    setColor(186, 28, 31, 83);
    setColor(187, 28, 47, 83);
    setColor(188, 28, 63, 83);
    setColor(189, 28, 79, 83);
    setColor(190, 28, 94, 83);
    setColor(191, 28, 110, 83);
    setColor(192, 55, 0, 83);
    setColor(193, 55, 16, 83);
    setColor(194, 55, 31, 83);
    setColor(195, 55, 47, 83);
    setColor(196, 55, 63, 83);
    setColor(197, 55, 79, 83);
    setColor(198, 55, 94, 83);
    setColor(199, 55, 110, 83);
    setColor(200, 83, 0, 83);
    setColor(201, 83, 16, 83);
    setColor(202, 83, 31, 83);
    setColor(203, 83, 47, 83);
    setColor(204, 83, 63, 83);
    setColor(205, 83, 79, 83);
    setColor(206, 83, 94, 83);
    setColor(207, 83, 110, 83);
    setColor(208, 110, 0, 83);
    setColor(209, 110, 16, 83);
    setColor(210, 110, 31, 83);
    setColor(211, 110, 47, 83);
    setColor(212, 110, 63, 83);
    setColor(213, 110, 79, 83);
    setColor(214, 110, 94, 83);
    setColor(215, 110, 110, 83);
    setColor(216, 0, 0, 110);
    setColor(217, 0, 16, 110);
    setColor(218, 0, 31, 110);
    setColor(219, 0, 47, 110);
    setColor(220, 0, 63, 110);
    setColor(221, 0, 79, 110);
    setColor(222, 0, 94, 110);
    setColor(223, 0, 110, 110);
    setColor(224, 28, 0, 110);
    setColor(225, 28, 16, 110);
    setColor(226, 28, 31, 110);
    setColor(227, 28, 47, 110);
    setColor(228, 28, 63, 110);
    setColor(229, 28, 79, 110);
    setColor(230, 28, 94, 110);
    setColor(231, 28, 110, 110);
    setColor(232, 55, 0, 110);
    setColor(233, 55, 16, 110);
    setColor(234, 55, 31, 110);
    setColor(235, 55, 47, 110);
    setColor(236, 55, 63, 110);
    setColor(237, 55, 79, 110);
    setColor(238, 55, 94, 110);
    setColor(239, 55, 110, 110);
    setColor(240, 83, 0, 110);
    setColor(241, 83, 16, 110);
    setColor(242, 83, 31, 110);
    setColor(243, 83, 47, 110);
    setColor(244, 83, 63, 110);
    setColor(245, 83, 79, 110);
    setColor(246, 83, 94, 110);
    setColor(247, 83, 110, 110);
    setColor(248, 110, 0, 110);
    setColor(249, 110, 16, 110);
    setColor(250, 110, 31, 110);
    setColor(251, 110, 47, 110);
    setColor(252, 110, 63, 110);
    setColor(253, 110, 79, 110);
    setColor(254, 110, 94, 110);
    setColor(255, 110, 110, 110);
    setColor(0, 140, 140, 140);
    setColor(1, 180, 40, 40);
    setColor(2, 0, 120, 0);
    setColor(4, 100, 100, 240);
    setColor(5, 160, 60, 160);
    setColor(9, 100, 0, 0);
    setColor(13, 60, 20, 60);
    setColor(15, 200, 50, 50);
    setColor(59, 40, 140, 40);
    setColor(90, 160, 120, 40);
    setColor(94, 120, 80, 20);
    setColor(95, 140, 140, 100);
    setColor(124, 90, 68, 30);
    setColor(254, 50, 70, 70);
    setColor(255, 70, 70, 70);
}