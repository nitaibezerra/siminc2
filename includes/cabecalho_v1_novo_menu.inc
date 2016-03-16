<?PHP
    /**
     * Carrega os sistemas que os usuarios com os cpf passado tem acesso.
     *
     * @global object $db
     * @return array
     */
    function carregarSistemas() {
        global $db;

        if ($_SESSION['usucpf'] != $_SESSION['usucpforigem']) {

            $sql = sprintf(
                "SELECT s.sisid,
                        trim(s.sisabrev) as sisabrev,
                        trim(s.sisdsc) as sisdsc

                FROM seguranca.usuario u

                INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
                INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
                INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
                INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf

                WHERE   u.usucpf = '%s' AND
                        us.suscod = 'A' AND
                        p.pflstatus = 'A' AND
                        u.suscod = 'A' AND
                        s.sisstatus = 'A' AND
                        s.sisid = %d
                GROUP BY s.sisid, s.sisabrev, s.sisdsc
                ORDER BY s.sisabrev", $_SESSION['usucpf'], $_SESSION['sisid']
            );
        } else {
            $sql = sprintf(
                "SELECT s.sisid,
                        trim(s.sisabrev) as sisabrev,
                        trim(s.sisdsc) as sisdsc

                FROM seguranca.usuario u
                INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
                INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
                INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
                INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf

                WHERE   u.usucpf = '%s' AND
                        us.suscod = 'A' AND
                        p.pflstatus = 'A' AND
                        s.sisstatus = 'A' AND
                        u.suscod = 'A'
                GROUP BY s.sisid, s.sisabrev, s.sisdsc
                ORDER BY s.sisabrev", $_SESSION['usucpf']
            );
        }

        $sistemas = $db->carregar($sql);

        return $sistemas;
    }

    /**
     * Recupera um array com os menus e seus itens de menus de maneira recursivamente.
     *
     * @global object $db - class
     * @param integer $sisId
     * @param string $cpf
     * @return array $menus
     */
    function carregarMenus($sisId, $cpf) {
        global $db;

        // Carregando o menu e itens de menu deste sistema e dos usuarios especificos.
        $lista_cpf = "'" . $cpf . "'";
        $sql = "
            select  distinct mnu.mnucod,
                    mnu.mnuid,
                    mnu.mnuidpai,
                    mnu.mnudsc,
                    mnu.mnustatus,
                    mnu.mnulink,
                    mnu.mnutipo,
                    mnu.mnustile,
                    mnu.mnuhtml,
                    mnu.mnusnsubmenu,
                    mnu.mnutransacao,
                    mnu.mnushow,
                    mnu.abacod
            from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu

            where   mnu.mnuid=pmn.mnuid
                    and pmn.pflcod=pfl.pflcod
                    and pfl.pflcod=pfu.pflcod
                    and pfu.usucpf in (" . $lista_cpf . ")
                    and ( mnu.mnutipo=1 or mnu.mnuidpai is not null )
                    and mnu.mnushow='t' and mnu.mnustatus='A'
                    and mnu.sisid=" . $sisId . "
            order by mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc
        ";
        $menusBd = @$db->carregar($sql);

        // Monta o array com os menus e seus itens de menus de maneira recursivamente.
        $menus = array();
        foreach ($menusBd as $key => &$menuBd) {
            if (empty($menuBd['mnuidpai'])) {
                $menus[$menuBd['mnuid']] = $menuBd;
                unset($menusBd[$key]);

                // Carrega os itens de menu deste menu principal.
                $menus[$menuBd['mnuid']]['itensMenu'] = tratarMenus($menusBd, $menuBd);
            }
        }
        return $menus;
    }

    function tratarMenus($menusBd, $menuPaiBd, $key = null) {

        $menus = array();
        foreach ($menusBd as $key => $menuBd) {
            if (!empty($menuBd['mnuidpai']) && $menuPaiBd['mnuid'] == $menuBd['mnuidpai']) {
                $menus[$menuBd['mnuid']] = $menuBd;
                unset($menusBd[$key]);
                $menus[$menuBd['mnuid']]['itensMenu'] = tratarMenus($menusBd, $menuBd, $key);
            }
        }

        return $menus;
    }

    /**
     * Gera o html do menu e chamando a funcao de gerar os itens menus recursivamente de maneira infitina.
     *
     * @name menuHtml
     * @return string
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.com>
     * @since 02/09/2013
     */
    function menuHtml(){
        //Recupera o menu de acordo com o sistema e usuario.
        $menus = carregarMenus($_SESSION['sisid'], $_SESSION['usucpf']);

        //Monta o html com os menus principais. Se tiver este menu tiver itens chama o metodo de montar o html de itens de menu com todos os itens deste menu principal.
        $menusHtml = '';
        foreach ($menus as $menu) {

            if (empty($menu['mnuidpai'])) {
                if ($menu['itensMenu'] && count($menu['itensMenu']) > 0) {
                    $menusHtml .= '<li class="dropdown">';
                    $menusHtml .= '<a class="dropdown-toggle" href="#" data-toggle="dropdown">';
                    $menusHtml .= $menu['mnuhtml'];
                    $menusHtml .= '<b class="caret"></b>';
                    $menusHtml .= '</a>';
                    $menusHtml .= '<ul class="dropdown-menu">';

                    // Monta o html com os itens deste menu principal recursivamente de maneira infinita.
                    $menusHtml .= menuItemHtml($menu['itensMenu']);

                    $menusHtml .= '</ul>';
                    $menusHtml .= '</li>';
                } else {
                    $menusHtml .= '<li>';
                    $menusHtml .= '<a href="#">';
                    $menusHtml .= $menu['mnuhtml'];
                    $menusHtml .= '</a>';
                    $menusHtml .= '</li>';
                }

                $menusHtml .= "";
            }
        }

        return $menusHtml;
    }

    /**
     * Monta o html de itens e chama ele mesmo caso tenha filhos.
     * De maneira recursivamente chamando ele mesmo e montado o html de todos os itens de menu.
     *
     * @param array $itensMenu - Array com os itens de menu.
     * @return string
     *
     * @author Ruy Junior Ferreira Silva <ruy.silva@gmail.com>
     * @since 03/09/2013
     */
    function menuItemHtml($itensMenu) {
        $menusHtml = '';
        foreach ($itensMenu as $menu) {

            // Se tiver itens dentro deste item de menu, cria o html deste menu do jeito q tera os itens dentro dele e chamando o proprio metodo para montar o html com os itens.
            if ($menu['itensMenu'] && count($menu['itensMenu']) > 0) {
                $url = ($menu['mnulink']) ? $menu['mnulink'] : '#';

                $menusHtml .= '<li class="dropdown-submenu">';
                $menusHtml .= '<a href="' . $url . '" tabindex="-1">' . $menu['mnutransacao'];
                $menusHtml .= '<ul class="dropdown-menu">';

                // Caso tenha mais itens de menu, chama o mesmo metodo para montar o html destes itens menus de maneira recursivamente até acabar os itens.
                $menusHtml .= menuItemHtml($menu['itensMenu']);

                $menusHtml .= '</ul>';
                $menusHtml .= '</a>';
                $menusHtml .= '</li>';

                // Se não tiver itens de menu ele cria o html no formato de item de menu sem mais itens dentro dele.
            } else {
                $url = ($menu['mnulink']) ? $menu['mnulink'] : '#';
                $menusHtml .= '<li>';
                $menusHtml .= '<a href="' . $url . '">' . $menu['mnutransacao'] . '</a>';
                $menusHtml .= '</li>';
            }
        }

        return $menusHtml;
    }

    // Definindo o tema do sistema.
    if (isset($_POST['theme'])) {
        $theme = $_POST['theme'];
        setcookie("theme", $theme, time() + 60 * 60 * 24 * 30, "/");
    } else if (isset($_COOKIE["theme"])) {
        $theme = $_COOKIE["theme"];
    } else {
        $theme = '';
    }

    $arrTheme = array('default', 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti', 'google');

    // Carregando array com os sisteams para o input select
    $sistemas = carregarSistemas();

    // Carregando o html do menu.
    $menus = menuHtml();



    //    if($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't'){
    //        $sql = "select schemaname , relname from pg_stat_user_tables
    //                where schemaname = '{$_SESSION['sisdiretorio']}'
    //                and relname = 'programacaoexercicio'
    //                order by relname;";
    //        $tabelaExercicio = $db->pegaLinha($sql);
    //
    //        if($tabelaExercicio){
    //            $sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from ". $_SESSION['sisdiretorio'] .".programacaoexercicio order by 1";
    //            $arrAnoExercicio = $db->carregar($sql);
    //        }
    //    } else {
    //        $arrAnoExercicio = array();
    //    }


    if ($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't') {
        $sql = "
            select  schemaname,
                    relname
            from pg_stat_user_tables

            where   schemaname = '{$_SESSION['sisdiretorio']}' and relname = 'programacaoexercicio'
            order by relname;
        ";
        $tabelaExercicio = $db->pegaLinha($sql);

        if ($tabelaExercicio) {
            $sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from " . $_SESSION['sisdiretorio'] . ".programacaoexercicio order by 1";
            $arrAnoExercicio = $db->carregar($sql);
        }
    }

    if (!$_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't' && $tabelaExercicio) {
        $sql = sprintf(
            "SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_SESSION['exercicio']
        );
        $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
    }

    // altera o ano de exercício (caso o usuário solicite)
    if ($_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio']) {

        if ($arrAnoExercicio) {
            foreach ($arrAnoExercicio as $anoExercicio) {
                $arrAno[] = $anoExercicio['codigo'];
                if ($anoExercicio['prsexerccorrente'] == "t") {
                    $ano_corrente = $anoExercicio['codigo'];
                }
            }
        }
        if (is_array($arrAno)) {
            if (in_array($_REQUEST['exercicio'], $arrAno)) {
                $_SESSION['exercicio'] = $_REQUEST['exercicio'];
            } else {
                $_SESSION['exercicio'] = $ano_corrente;
            }
        }
        $exercicio = $_SESSION['exercicio'];

        $sql = sprintf(
            "SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_REQUEST['exercicio']
        );
        $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
    }

    // pega a página inicial do sistema
    $sql = sprintf(
        "SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']
    );
    $paginic = $db->pegaUm($sql);
    
    include APPRAIZ . "includes/registraracesso.php";
    
?>
<!DOCTYPE html>

<html>
    <head>
        <title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle</title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>

        <link rel="stylesheet" href="http://blueimp.github.io/Gallery/css/blueimp-gallery.min.css">

        <!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload.css">
        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui.css">

        <!-- Bootstrap CSS -->
        <link rel="stylesheet" media="screen" href="../library/bootstrap-3.0.0/css/bootstrap.min.css">

        <!-- CSS adjustments for browsers with JavaScript disabled -->
        <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-noscript.css"></noscript>
        <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui-noscript.css"></noscript>


        <!--jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css-->
        <link rel='stylesheet' type='text/css' href='../includes/listagem2.css'/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem2.css'/>

        <link rel="stylesheet" media="screen" href="../library/chosen-1.0.0/chosen.css">


        <!-- jQuery JS -->
        <!-- <script src="../estrutura/js/funcoes.js" type="text/javascript" ></script> -->
        <script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>

<?PHP
    if ($theme && in_array($theme, $arrTheme)){
        echo '<link href="../library/bootstrap-3.0.0/css/bootstrap-theme-' . $theme . '.css" rel="stylesheet" media="screen">';
    }
?>
        <!-- Bootstrap JS -->
        <script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>



        <!-- Chosen CSS -->
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <style type="text/css" media="all">
            /* fix rtl for demo */
            .chosen-rtl .chosen-drop { left: -9000px; }
        </style>

        <!-- Custom CSS -->
        <link href="/library/simec/css/custom.css" rel="stylesheet" media="screen">
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">



        <style type="text/css">
            .posicaoDivMenu{
                top: 25px;
                border-top-color: #F5F5F5;
                background: #F5F5F5;
                z-index:0;
                /*background: #;*/
                /*background: linear-gradient(to bottom, #FFFFFF 15%, #FFFFFF 50%);*/
            }

            .div_barra_cabecalho{
                height: 25px;
                /*background: #f1f1f1;*/
                /*background: #DCDCDC;*/
                background: linear-gradient(to bottom, #DCDCDC 25%, #F5F5F5 85%);
                font-family: openSans, Verdana, sans-serif !important;
                font-size: 12px;
                line-height: 25px;
            }

            .div_flag_brasil{
                margin-left: 13%;
                width: 7%;
                height: 24px;
                border-right: 1px solid #dfdfdf;
                background: url(../imagens/bandeiras/flag_brasil.png) 8px center no-repeat;
                padding-left: 41px;
            }

            .acesso_informacao{
                position: absolute;
                top: 0px;
                margin-left: 22%;
                width: 24%;
                height: 25px;
                border-right: 1px solid #dfdfdf;
            }

            .div_ano_exercicio{
                position: absolute;
                top: 0px;
                margin-left: 44%;
                width: 15%;
                height: 25px;
                border-right: 1px solid #dfdfdf;
                padding-left: 55px;
            }

            .div_thema{
                position: absolute;
                top: 0px;
                margin-left: 60%;
                width: 11%;
                height: 25px;
                /*background: url(../imagens/cabecalho/monitor_thema_16x16.png) 22px center no-repeat;*/
                border-right: 1px solid #dfdfdf;
                padding-left: 10px;
            }

            .div_impressao{
                position: absolute;
                top: 0px;
                margin-left: 71%;
                width: 9%;
                height: 25px;
                /*background: url(../imagens/cabecalho/impressora_16x16.png) 22px center no-repeat;*/
                border-right: 1px solid #dfdfdf;
                padding-left: 30px;
            }

            .div_ajuda{
                position: absolute;
                top: 0px;
                margin-left: 79%;
                width: 7%;
                height: 25px;
                /*background: url(../imagens/cabecalho/ajuda_2_16x16.png) 22px center no-repeat;*/
                border-right: 1px solid #dfdfdf;
                padding-left: 30px;
            }

            .div_email{
                position: absolute;
                top: 0px;
                margin-left: 86%;
                width: 7%;
                height: 25px;
                /*background: url(../imagens/cabecalho/email_16x16.png) 22px center no-repeat;*/
                border-right: 1px solid #dfdfdf;
                padding-left: 30px;
            }

            .div_sair_sistema{
                position: absolute;
                top: 0px;
                margin-left: 93%;
                width: 6%;
                height: 25px;
                /*background: url(../imagens/cabecalho/sair_16x16.png) 22px center no-repeat;*/
                padding-left: 30px;
            }

            .titulo_link_acesso{
                color: #606060;
                /*padding-left: 42px;*/
                text-decoration: none;
                text-transform: uppercase;
            }
            
            .div_cronometro{
                color: #606060;
                font-size: 10px;
                /*padding-left: 42px;*/
                text-decoration: none;
                text-transform: uppercase;
            }

            a.chosen-single {

            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            /*background-color: #FFFFFF !important;*/
            /*border: 1px solid #AAAAAA !important;*/
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            /*color: #444444 !important;*/
            display: block !important;
            height: 25px !important;
            line-height: 25px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
            /*text-align: center !important;*/
            text-decoration: none !important;
            white-space: nowrap !important;
        }

        .chosen-container-multi .chosen-choices li.search-field input[type="text"] {
            margin: 1px 0;
            padding: 5px !important;
            height: 25px !important;
            outline: 0 !important;
            border: 0 !important;
            background: transparent !important;
            box-shadow: none !important;
            color: #666 !important;
            font-size: 100% !important;
            font-family: sans-serif !important;
            line-height: normal !important;
            border-radius: 0 !important;
        }

        .chosen-container-active.chosen-with-drop .chosen-single {
            border: 1px solid #aaa!important;
            -moz-border-radius-bottomright: 0 !important;
            border-bottom-right-radius: 0 !important;
            -moz-border-radius-bottomleft: 0 !important;
            border-bottom-left-radius: 0 !important;
            background-image: -webkit-gradient(linear, 50% 0%, 50% 100%, color-stop(20%, #eeeeee), color-stop(80%, #ffffff))!important;
            background-image: -webkit-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: -moz-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: -o-linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            background-image: linear-gradient(#eeeeee 20%, #ffffff 80%)!important;
            box-shadow: 0 1px 0 #fff inset!important;
        }

        .cabecalho a.chosen-single{
            -webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;
            background-color: #FFFFFF !important;
            border: 1px solid #AAAAAA !important;
            border-radius: 5px !important;
            box-shadow: 0 0 3px #FFFFFF inset, 0 1px 1px rgba(0, 0, 0, 0.1) !important;
            color: #444444 !important;
            display: block !important;
            height: 45px !important;
            line-height: 42px !important;
            overflow: hidden !important;
            padding: 0 0 0 8px !important;
            position: relative !important;
/*            text-align: center !important;*/
            text-decoration: none !important;
            white-space: nowrap !important;
            font-weight: bold;
            font-size: 22px;

            /*background-color: red !important;*/
            /*border: 1px solid #AAAAAA !important;*/
            /*color: #444444 !important;*/
            /*text-align: center !important;*/
        }

        </style>

    </head>
    <body>
        <!--
        <form id="form_system" method="post" action="/muda_sistema.php">
            <input type="hidden" name="sisid" id="sisid" value="" />
        </form>
        -->

        <form id="form_theme" method="post" action="">
            <input type="hidden" name="theme" id="theme" value="" />
        </form>

        <script lang="javascript">
            function changeSystem(system){
                //alert(system);
                location.href = "../muda_sistema.php?sisid=" + system;
                //$('#sisid').val(system);
                //$('#form_system').submit();
            }

            function changeTheme(theme){
                $('#theme').val(theme);
                $('#form_theme').submit();
            }

            function simularUsuario(){
                var horizontal = 550;
                var vertical   = 320;

                var res_ver = screen.height;
                var res_hor = screen.width;

                var pos_ver_fin = (parseInt(res_ver) - parseInt(vertical) )/2;
                var pos_hor_fin = (parseInt(res_hor) - parseInt(horizontal) )/2;

                //displayMessage('../includes/simularUsuario.php');
                return window.open("../includes/simularUsuario.php","Simular Usuário","width="+horizontal+",height="+vertical+",top="+pos_ver_fin+",left="+pos_hor_fin);

                //window.open("../includes/simularUsuario.php", "PTA","menubar=no,toolbar=no,scrollbars=no,resizable=no,left=20,top=20,width=560,height=345");
                //displayMessage('../includes/simularUsuario.php');
            }
        </script>

        <div class="div_barra_cabecalho navbar-fixed-top">
            <div class="div_flag_brasil">
                <span class="titulo_link_acesso">Brasil</span>
            </div>
            <div class="acesso_informacao">
                <span class="glyphicon glyphicon-info-sign"></span>
                <span class="titulo_link_acesso"> Acesso à informação </span>
            </div>
            <div class="div_ano_exercicio">
                <span class="glyphicon glyphicon-calendar"></span>
                <span class="titulo_link_acesso">Exercicio</span>

                <div class="btn-group">
                    <form id="setperfil" name="setperfil" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">
                        <input type="hidden" id="exercicio" name="exercicio" value=""  class="form-control" />
                    </form>
                    <?php if ($arrAnoExercicio && is_array($arrAnoExercicio)): ?>
                        <div class="btn-group" style="z-index:9999;">
                            <button class="btn btn-sm btn-default dropdown-toggle" data-toggle="dropdown" style="height: 20px; width: 75px; padding-top: 0px;">
                                <span>
                                   <?php echo $_SESSION['exercicio'] ?> &nbsp;
                                </span>
                                <span class="caret"> &nbsp; </span>
                            </button>
                            <ul class="dropdown-menu">
                                <li role="presentation" class="dropdown-header">
                                    <i class="glyphicon glyphicon-calendar"></i> Exercício
                                </li>
                                <li role="presentation" class="divider"></li>
                                <?php foreach ($arrAnoExercicio as $anoExercicio): ?>
                                    <li>
                                        <a href="#" onclick="javascript:alteraTema(<?php echo $anoExercicio['codigo'] ?>)">
                                            <!-- <i class="glyphicon glyphicon-calendar"></i> -->
                                            <?php echo $anoExercicio['descricao'] ?>
                                        </a>
                                    </li>
                                <?php endforeach; ?>
                            </ul>
                            <script>
                                function alteraTema(id) {
                                    $('#exercicio').val(id);
                                    document.getElementById('setperfil').submit();
                                }
                            </script>
                        </div>
                    <?php endif ?>
                </div>
            </div>

            <div class="div_thema">
                <span class="glyphicon glyphicon-picture"></span>
                <span class="titulo_link_acesso"> Tema </span>

                <div class="btn-group" style="z-index:9999;">
                    <button class="btn btn-sm btn-default dropdown-toggle" data-toggle="dropdown" style="font-size: 11px; height: 20px; width: 90px; padding-top: 0px;">
                        <span>
                            <?php echo strtoupper($theme); ?> &nbsp;
                         </span>
                        <span class="caret"></span>
                    </button>
                    <ul class="dropdown-menu">
                        <li role="presentation" class="dropdown-header">
                            <i class="glyphicon glyphicon-picture"></i> Temas
                        </li>
                        <li role="presentation" class="divider"></li>
                        <li>
                            <?php foreach ($arrTheme as $theme): ?>
                                <a href="#" onclick="javascript:changeTheme('<?php echo $theme; ?>');">
                                    <?php echo ucfirst($theme) ?>
                                </a>
                            <?php endforeach; ?>
                        </li>
                    </ul>
                </div>
            </div>

            <div class="div_impressao">
                <!--<a href="http://brasil.gov.br/barra#acesso-informacao" class="link_acesso"></a>-->
                <span class="glyphicon glyphicon-print"></span>
                <span class="titulo_link_acesso"> Impressão </span>

            </div>

            <div class="div_ajuda">
                <!-- <a href="http://brasil.gov.br/barra#acesso-informacao" class="link_acesso">Ajuda</a> -->
                <span class="glyphicon glyphicon-question-sign"></span>
                <span class="titulo_link_acesso"> Ajuda </span>
            </div>

            <div class="div_email">
                <!-- <a href="http://brasil.gov.br/barra#acesso-informacao" class="link_acesso">E-mail</a> -->
                <span class="glyphicon glyphicon-envelope"></span>
                <span class="titulo_link_acesso"> E-mail </span>
            </div>

            <div class="div_sair_sistema">
                <!-- <a href="http://brasil.gov.br/barra#acesso-informacao" class="link_acesso">Sair</a> -->
                <span class="glyphicon glyphicon-off"></span>
                <span class="titulo_link_acesso"> Sair </span>
            </div>
        </div>

        <div class="navbar navbar-default navbar-fixed-top posicaoDivMenu">

            <div class="navbar-header" style="width:36%; margin-top:17px;">
                <ul class="nav">
                    <li>
                        <a class="navbar-brand" href="#" onclick="javascript:changeSystem(<?php echo $_SESSION['sisid'] ?>)">
                            <!--SiMEC-->
                            <img width="110px" src="../includes/layout/planeta/img/logo.png">
                        </a>
                        <div class="collapse navbar-collapse" style="margin-left: 5px;">
                            <ul class="nav navbar-nav">
                                <?php echo $menus ?>
                            </ul>
                        </div>
                    </li>
                </ul>
            </div>

            <div class="navbar-brand" style="width:35%; margin-top:5px; margin-left: 15px;">
                <div class="cabecalho">
                    <select data-placeholder="Escolha um módulo do sistema..." id="cabecalho" class="chosen-select" style="width:400px;" tabindex="2" onchange="javascipt:changeSystem(this.value);">
                        <?php
                        if($sistemas && is_array($sistemas)){
                            foreach ($sistemas as $sistema): ?>
                                <option <?php if($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                            <?php endforeach;
                        } ?>
                    </select>
                </div>

                <script type="text/javascript">
                    var jq = jQuery.noConflict();
                    jq('.chosen-select').chosen({allow_single_deselect:true});
                </script>
            </div>

            <form id="setperfil2" name="setperfil2" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
                <input type="hidden" name="usucpf_simu" id="usrs" value="<?php echo $usucpf_simu;?>">

                <div class="navbar-brand" style=" margin-top:3px; margin-left: 120px;">
                    <div class="collapse navbar-collapse">
                        <button class="btn btn-sm btn-default">
                            <i class="glyphicon glyphicon-user"></i>
                                <?php echo $_SESSION['usunome'] ?>
                        </button>
                    </div>
                    
                    <div class="div_cronometro" id="cronometro_div" style="margin-top: 0px; margin-left: 46px;">Sua sessão expira em:<span id="cronometro" style="font-weight:bold; color: #f00;"></span></div>
                    
                </div>

                <div class="btn-group" style="margin-top: 3px;">
                    <button class="btn btn-sm btn-default dropdown-toggle" data-toggle="dropdown" style="font-size: 11px; height: 30px; width: 12px; margin-top: 15px; margin-left: -35px;">
                        <span class="glyphicon glyphicon-refresh"></span>
                        <a href="#" onclick="javascript:simularUsuario();"></a>
                    </button>
                </div>
            </form>
        </div>

        <!--
        <div class="navbar navbar-default navbar-fixed-top" style="top: 55px;z-index:0;">
        -->
            <?PHP
                //if ($modulo_atual != ""){
                    $sql = sprintf(
                        "select distinct mnu1.mnudsc as menu1  , mnu2.mnudsc as menu2 , mnu3.mnudsc as menu3 , mnu1.mnulink as mnulink1  , mnu2.mnulink as mnulink2 , mnu3.mnulink as mnulink3    from seguranca.menu mnu1, seguranca.menu mnu2,  seguranca.menu mnu3 where mnu3.mnuidpai=mnu2.mnuid and mnu2.mnuidpai=mnu1.mnuid and mnu3.mnuid = %d", $modulo_atual
                    );
                    $rastro = $db->pegaLinha($sql);
                    
                    if ($rastro){
            ?>
                        <div class="row">
                            <div class="col-md-12">
                                <div class="view">
                                    <ul class="breadcrumb">
                                        <li>
                                            <?php   if ($rastro['menulink1']){ ?>

                                                <a <?php if ($url == $rastro['menulink1']) echo 'class="active"'; echo "href='{$rastro['menulink1']}'"; ?> contenteditable="true"><?php echo $rastro['menu1'] ?></a>

                                            <?php   }else{
                                                        echo $rastro['menu1'];            
                                                    } 
                                            ?>
                                        </li>

                                        <li>
                                            <?php   if ($rastro['menulink2']){ ?>
                                                        <a <?php if ($url == $rastro['menulink2']) echo 'class="active"'; echo "href='{$rastro['menulink2']}'"; ?> contenteditable="true"><?php echo $rastro['menu2'] ?></a>    
                                            <?php   }else{
                                                        echo $rastro['menu2'];                                                                    
                                                    } 
                                            ?>
                                        </li>
                                        <li>
                                            <?php   if($rastro['menulink3']){ ?>
                                                        <a <?php if ($url == $rastro['menulink3']) echo 'class="active"'; echo "href='{$rastro['menulink3']}'"; ?> contenteditable="true"><?php echo $rastro['menu3'] ?></a>
                                            <?php   }else{
                                                        echo $rastro['menu3'];
                                                    }           
                                            ?>
                                            <span class="divider"></span>
                                        </li>
                                    </ul>
                    <?php } ?>
                        </div>
                    </div>
                </div>
                <? //} ?>
        </div>

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

<script type="text/javascript">

var minutos=<? echo floor((MAXONLINETIME / 60)); ?>;
var seconds=<? echo floor(MAXONLINETIME % 60); ?>;
var campo = document.getElementById("cronometro");
var campo_div = document.getElementById("cronometro_div");

function startCountdown(){
    if (seconds<=0){
        seconds=60;
        minutos-=1;
     }
     if (minutos<=-1){
        seconds=0;
        seconds+=1;
        campo.innerHTML="";
        campo_div.innerHTML="Sessão expirada!";
            location.href = "../../logout.php";
     } else{
            seconds-=1
            if(seconds < 10) {
                    seconds = "0" + seconds;
            }
        campo.innerHTML = " " + minutos+"min"+seconds+"s";
        setTimeout("startCountdown()",1000);
    }
}
startCountdown();

</script>