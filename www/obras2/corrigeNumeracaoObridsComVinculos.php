<?php

set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

include_once '../obras2/_funcoes.php';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//corrigeNumeracaoIdsComVinculos();

// Arquivo executado no dia 04/12/2013

// Resultado obtido:

/**
 Array
(
    [0] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1776 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005279 => obridvinculado:  => 1776
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900101776 => obridvinculado:  => 1776
                    [obra_nova] => obrid: 1776 => obridvinculado:  => 900101776
                )

        )

    [1] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1805 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001210 => obridvinculado:  => 1805
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900101805 => obridvinculado:  => 1805
                    [obra_nova] => obrid: 1805 => obridvinculado:  => 900101805
                )

        )

    [2] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1833 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1000354 => obridvinculado:  => 1833
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900101833 => obridvinculado:  => 1833
                    [obra_nova] => obrid: 1833 => obridvinculado:  => 900101833
                )

        )

    [3] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1925 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006895 => obridvinculado:  => 1925
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900101925 => obridvinculado:  => 1925
                    [obra_nova] => obrid: 1925 => obridvinculado:  => 900101925
                )

        )

    [4] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1965 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1009571 => obridvinculado:  => 1965
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900101965 => obridvinculado:  => 1965
                    [obra_nova] => obrid: 1965 => obridvinculado:  => 900101965
                )

        )

    [5] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1965 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1009570 => obridvinculado:  => 1965
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900201965 => obridvinculado:  => 1965
                    [obra_nova] => obrid: 1965 => obridvinculado:  => 900201965
                )

        )

    [6] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1965 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005679 => obridvinculado:  => 1965
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900301965 => obridvinculado:  => 1965
                    [obra_nova] => obrid: 1965 => obridvinculado:  => 900301965
                )

        )

    [7] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 1989 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001170 => obridvinculado:  => 1989
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900101989 => obridvinculado:  => 1989
                    [obra_nova] => obrid: 1989 => obridvinculado:  => 900101989
                )

        )

    [8] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 2053 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006897 => obridvinculado:  => 2053
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900102053 => obridvinculado:  => 2053
                    [obra_nova] => obrid: 2053 => obridvinculado:  => 900102053
                )

        )

    [9] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 2102 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001195 => obridvinculado:  => 2102
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900102102 => obridvinculado:  => 2102
                    [obra_nova] => obrid: 2102 => obridvinculado:  => 900102102
                )

        )

    [10] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 3827 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1000382 => obridvinculado:  => 3827
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900103827 => obridvinculado:  => 3827
                    [obra_nova] => obrid: 3827 => obridvinculado:  => 900103827
                )

        )

    [11] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 8358 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1003245 => obridvinculado:  => 8358
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900108358 => obridvinculado:  => 8358
                    [obra_nova] => obrid: 8358 => obridvinculado:  => 900108358
                )

        )

    [12] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 8603 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005464 => obridvinculado:  => 8603
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900108603 => obridvinculado:  => 8603
                    [obra_nova] => obrid: 8603 => obridvinculado:  => 900108603
                )

        )

    [13] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 8653 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1000424 => obridvinculado:  => 8653
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900108653 => obridvinculado:  => 8653
                    [obra_nova] => obrid: 8653 => obridvinculado:  => 900108653
                )

        )

    [14] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 8761 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005604 => obridvinculado:  => 8761
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900108761 => obridvinculado:  => 8761
                    [obra_nova] => obrid: 8761 => obridvinculado:  => 900108761
                )

        )

    [15] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 8808 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004342 => obridvinculado:  => 8808
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900108808 => obridvinculado:  => 8808
                    [obra_nova] => obrid: 8808 => obridvinculado:  => 900108808
                )

        )

    [16] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 8956 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005745 => obridvinculado:  => 8956
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900108956 => obridvinculado:  => 8956
                    [obra_nova] => obrid: 8956 => obridvinculado:  => 900108956
                )

        )

    [17] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 9096 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004050 => obridvinculado:  => 9096
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900109096 => obridvinculado:  => 9096
                    [obra_nova] => obrid: 9096 => obridvinculado:  => 900109096
                )

        )

    [18] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 9097 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1003901 => obridvinculado:  => 9097
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900109097 => obridvinculado:  => 9097
                    [obra_nova] => obrid: 9097 => obridvinculado:  => 900109097
                )

        )

    [19] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 9517 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1002050 => obridvinculado:  => 9517
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900109517 => obridvinculado:  => 9517
                    [obra_nova] => obrid: 9517 => obridvinculado:  => 900109517
                )

        )

    [20] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 11826 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1002154 => obridvinculado:  => 11826
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900111826 => obridvinculado:  => 11826
                    [obra_nova] => obrid: 11826 => obridvinculado:  => 900111826
                )

        )

    [21] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 11899 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004346 => obridvinculado:  => 11899
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900111899 => obridvinculado:  => 11899
                    [obra_nova] => obrid: 11899 => obridvinculado:  => 900111899
                )

        )

    [22] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 11919 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004818 => obridvinculado:  => 11919
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900111919 => obridvinculado:  => 11919
                    [obra_nova] => obrid: 11919 => obridvinculado:  => 900111919
                )

        )

    [23] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 11932 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006131 => obridvinculado:  => 11932
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900111932 => obridvinculado:  => 11932
                    [obra_nova] => obrid: 11932 => obridvinculado:  => 900111932
                )

        )

    [24] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 12653 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004092 => obridvinculado:  => 12653
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900112653 => obridvinculado:  => 12653
                    [obra_nova] => obrid: 12653 => obridvinculado:  => 900112653
                )

        )

    [25] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 13109 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1002538 => obridvinculado:  => 13109
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900113109 => obridvinculado:  => 13109
                    [obra_nova] => obrid: 13109 => obridvinculado:  => 900113109
                )

        )

    [26] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 13421 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005214 => obridvinculado:  => 13421
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900113421 => obridvinculado:  => 13421
                    [obra_nova] => obrid: 13421 => obridvinculado:  => 900113421
                )

        )

    [27] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 13754 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004712 => obridvinculado:  => 13754
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900113754 => obridvinculado:  => 13754
                    [obra_nova] => obrid: 13754 => obridvinculado:  => 900113754
                )

        )

    [28] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 17507 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006610 => obridvinculado:  => 17507
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900117507 => obridvinculado:  => 17507
                    [obra_nova] => obrid: 17507 => obridvinculado:  => 900117507
                )

        )

    [29] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 17619 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005486 => obridvinculado:  => 17619
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900117619 => obridvinculado:  => 17619
                    [obra_nova] => obrid: 17619 => obridvinculado:  => 900117619
                )

        )

    [30] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 18310 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001017 => obridvinculado:  => 18310
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900118310 => obridvinculado:  => 18310
                    [obra_nova] => obrid: 18310 => obridvinculado:  => 900118310
                )

        )

    [31] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 18471 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1002430 => obridvinculado:  => 18471
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900118471 => obridvinculado:  => 18471
                    [obra_nova] => obrid: 18471 => obridvinculado:  => 900118471
                )

        )

    [32] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19237 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004708 => obridvinculado:  => 19237
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119237 => obridvinculado:  => 19237
                    [obra_nova] => obrid: 19237 => obridvinculado:  => 900119237
                )

        )

    [33] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19368 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005375 => obridvinculado:  => 19368
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119368 => obridvinculado:  => 19368
                    [obra_nova] => obrid: 19368 => obridvinculado:  => 900119368
                )

        )

    [34] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19462 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001696 => obridvinculado:  => 19462
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119462 => obridvinculado:  => 19462
                    [obra_nova] => obrid: 19462 => obridvinculado:  => 900119462
                )

        )

    [35] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19539 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1000294 => obridvinculado:  => 19539
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119539 => obridvinculado:  => 19539
                    [obra_nova] => obrid: 19539 => obridvinculado:  => 900119539
                )

        )

    [36] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19676 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004820 => obridvinculado:  => 19676
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119676 => obridvinculado:  => 19676
                    [obra_nova] => obrid: 19676 => obridvinculado:  => 900119676
                )

        )

    [37] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19719 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006857 => obridvinculado:  => 19719
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119719 => obridvinculado:  => 19719
                    [obra_nova] => obrid: 19719 => obridvinculado:  => 900119719
                )

        )

    [38] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 19934 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004999 => obridvinculado:  => 19934
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900119934 => obridvinculado:  => 19934
                    [obra_nova] => obrid: 19934 => obridvinculado:  => 900119934
                )

        )

    [39] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 20012 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001264 => obridvinculado:  => 20012
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900120012 => obridvinculado:  => 20012
                    [obra_nova] => obrid: 20012 => obridvinculado:  => 900120012
                )

        )

    [40] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 20041 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001368 => obridvinculado:  => 20041
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900120041 => obridvinculado:  => 20041
                    [obra_nova] => obrid: 20041 => obridvinculado:  => 900120041
                )

        )

    [41] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 20132 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1003257 => obridvinculado:  => 20132
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900120132 => obridvinculado:  => 20132
                    [obra_nova] => obrid: 20132 => obridvinculado:  => 900120132
                )

        )

    [42] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 23237 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005603 => obridvinculado:  => 23237
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900123237 => obridvinculado:  => 23237
                    [obra_nova] => obrid: 23237 => obridvinculado:  => 900123237
                )

        )

    [43] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 24572 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004922 => obridvinculado:  => 24572
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900124572 => obridvinculado:  => 24572
                    [obra_nova] => obrid: 24572 => obridvinculado:  => 900124572
                )

        )

    [44] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 24881 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006703 => obridvinculado:  => 24881
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900124881 => obridvinculado:  => 24881
                    [obra_nova] => obrid: 24881 => obridvinculado:  => 900124881
                )

        )

    [45] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 25042 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1006917 => obridvinculado:  => 25042
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900125042 => obridvinculado:  => 25042
                    [obra_nova] => obrid: 25042 => obridvinculado:  => 900125042
                )

        )

    [46] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 25043 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004688 => obridvinculado:  => 25043
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900125043 => obridvinculado:  => 25043
                    [obra_nova] => obrid: 25043 => obridvinculado:  => 900125043
                )

        )

    [47] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 25043 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004689 => obridvinculado:  => 25043
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900225043 => obridvinculado:  => 25043
                    [obra_nova] => obrid: 25043 => obridvinculado:  => 900225043
                )

        )

    [48] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 25364 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1001038 => obridvinculado:  => 25364
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900125364 => obridvinculado:  => 25364
                    [obra_nova] => obrid: 25364 => obridvinculado:  => 900125364
                )

        )

    [49] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 26265 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004358 => obridvinculado:  => 26265
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900126265 => obridvinculado:  => 26265
                    [obra_nova] => obrid: 26265 => obridvinculado:  => 900126265
                )

        )

    [50] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 26348 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1004924 => obridvinculado:  => 26348
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900126348 => obridvinculado:  => 26348
                    [obra_nova] => obrid: 26348 => obridvinculado:  => 900126348
                )

        )

    [51] => Array
        (
            [o_que_era] => Array
                (
                    [obra_original] => obrid: 29692 => obridvinculado:  => NULL
                    [obra_nova] => obrid: 1005582 => obridvinculado:  => 29692
                )

            [como_ficou] => Array
                (
                    [obra_original] => obrid: 900129692 => obridvinculado:  => 29692
                    [obra_nova] => obrid: 29692 => obridvinculado:  => 900129692
                )

        )

)
 */
