<?php

    /**
     * Carrega os sistemas que os usuarios com os cpf passado tem acesso.
     * 
     * @global object $db
     * @return array
     */
    function carregarSistemas()
    {
        global $db;

        if ($_SESSION['usucpf'] != $_SESSION['usucpforigem'])
        {
            $sql = sprintf(
                    "SELECT
                                        s.sisid, trim(s.sisabrev) as sisabrev, trim(s.sisdsc) as sisdsc
                                        FROM seguranca.usuario u
                                        INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
                                        INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
                                        INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
                                        INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
                                WHERE
                                        u.usucpf = '%s' AND
                                        us.suscod = 'A' AND
                                        p.pflstatus = 'A' AND
                                        u.suscod = 'A' AND
                                        s.sisstatus = 'A' AND
                                        s.sisid = %d
                                GROUP BY s.sisid, s.sisabrev, s.sisdsc
                                ORDER BY s.sisabrev", $_SESSION['usucpf'], $_SESSION['sisid']
            );
        } else
        {
            $sql = sprintf(
                    "SELECT
                                        s.sisid, trim(s.sisabrev) as sisabrev, trim(s.sisdsc) as sisdsc
                                        FROM seguranca.usuario u
                                        INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
                                        INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod
                                        INNER JOIN seguranca.sistema s ON p.sisid = s.sisid
                                        INNER JOIN seguranca.usuario_sistema us ON s.sisid = us.sisid AND u.usucpf = us.usucpf
                                WHERE
                                        u.usucpf = '%s' AND
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
    function carregarMenus($sisId , $cpf)
    {
        global $db;

        // Carregando o menu e itens de menu deste sistema e dos usuarios especificos.
        $lista_cpf = "'" . $cpf . "'";
        $sql = "select distinct mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , 
                   mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod 
                   from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu 
                   where mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod 
                   and pfu.usucpf in (" . $lista_cpf . ") and ( mnu.mnutipo=1 or mnu.mnuidpai is not null )  and mnu.mnushow='t' and mnu.mnustatus='A' 
                   and mnu.sisid=" . $sisId . "  
                   order by mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc";

        $menusBd = @$db->carregar($sql);
        
        // Monta o array com os menus e seus itens de menus de maneira recursivamente.
        $menus = array();
        foreach ($menusBd as $key => &$menuBd) {
            if (empty($menuBd['mnuidpai']))
            {
                $menus[$menuBd['mnuid']] = $menuBd;
                unset($menusBd[$key]);
                
                // Carrega os itens de menu deste menu principal.
                $menus[$menuBd['mnuid']]['itensMenu'] = tratarMenus($menusBd, $menuBd);
            }
        }

        return $menus;
    }

    function tratarMenus($menusBd, $menuPaiBd, $key = null)
    {

        $menus = array();
        foreach ($menusBd as $key => $menuBd) {
            if (!empty($menuBd['mnuidpai']) && $menuPaiBd['mnuid'] == $menuBd['mnuidpai'])
            {
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
    function menuHtml()
    {
        // Recupera o menu de acordo com o sistema e usuario.
        $menus = carregarMenus( $_SESSION['sisid'] , $_SESSION['usucpf']);
        
        // Monta o html com os menus principais
        // Se tiver este menu tiver itens chama o metodo de montar o html de itens de menu com todos os itens deste menu principal.
        $menusHtml = '';
        foreach($menus as $menu){
            
            if(empty($menu['mnuidpai'])){
                if($menu['itensMenu'] && count($menu['itensMenu']) > 0){
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
    function menuItemHtml($itensMenu)
    {
        $menusHtml = '';
        foreach($itensMenu as $menu){
            
            // Se tiver itens dentro deste item de menu, cria o html deste menu do jeito q tera os itens dentro dele e chamando o proprio metodo para montar o html com os itens.
            if($menu['itensMenu'] && count($menu['itensMenu']) > 0 ){
                    $url = ($menu['mnulink'])? $menu['mnulink'] : '#';
                    
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
                $url = ($menu['mnulink'])? $menu['mnulink'] : '#';
                $menusHtml .= '<li>';
                $menusHtml .= '<a href="' . $url . '">' . $menu['mnutransacao'] . '</a>';
                $menusHtml .= '</li>';
            }
        }
        
        return $menusHtml;
    }
    
    // Definindo o tema do sistema.
    if (isset($_POST['theme']))
    {
        $theme = $_POST['theme'];
        setcookie("theme", $theme, time() + 60 * 60 * 24 * 30, "/");
    } else if (isset($_COOKIE["theme"]))
    {
        $theme = $_COOKIE["theme"];
    } else
    {
        $theme = '';
    }

    $arrTheme = array('default' , 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united');

    // Carregando array com os sisteams para o input select
    $sistemas = carregarSistemas();

    // Carregando o html do menu.
    $menus = menuHtml();
    ?>
    <!DOCTYPE html>

    <html>
        <head>
            <title><?php echo NOME_SISTEMA; ?></title>
            <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">

            <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-uiq'/>
            <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>

<!--            <link rel="stylesheet" href="http://blueimp.github.io/Gallery/css/blueimp-gallery.min.css">-->
            <!-- Bootstrap File CSS -->
<!--            <link rel='stylesheet' type='text/css' href='../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload.css'/>-->
<!--            <link rel='stylesheet' type='text/css' href='../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui.css'/>-->
<!--            <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-noscript.css"></noscript>-->
<!--            <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui-noscript.css"></noscript>-->


            <!-- Force latest IE rendering engine or ChromeFrame if installed -->
            <!--[if IE]>
                <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
            <![endif]-->
<!--            <meta name="description" content="File Upload widget with multiple file selection, drag&amp;drop support, progress bars, validation and preview images, audio and video for jQuery. Supports cross-domain, chunked and resumable file uploads and client-side image resizing. Works with any server-side platform (PHP, Python, Ruby on Rails, Java, Node.js, Go etc.) that supports standard HTML form file uploads.">-->
<!--            <meta name="viewport" content="width=device-width, initial-scale=1.0">-->
            <!-- Bootstrap styles -->
<!--            <link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">-->
            <!-- Generic page styles -->
<!--            <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/style.css">-->
            <!-- blueimp Gallery styles -->
            <link rel="stylesheet" href="http://blueimp.github.io/Gallery/css/blueimp-gallery.min.css">
            <!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
            <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload.css">
            <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui.css">
            <!-- CSS adjustments for browsers with JavaScript disabled -->
            <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-noscript.css"></noscript>
            <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui-noscript.css"></noscript>

            <!-- Bootstrap CSS -->
            <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
            
            <?php 
//            var_dump(file_exists('../estrutura/js/funcoes.js'));
//            exit;
            ?>
            <!--jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css-->
            <!--<link rel='stylesheet' type='text/css' href='../includes/listagem2.css'/>-->
            <!--<link rel='stylesheet' type='text/css' href='../includes/listagem2.css'/>-->

            <!-- jQuery JS -->
            <script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script src="../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script src="../library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script language="JavaScript" src="../estrutura/js/funcoes.js"></script>
            <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
            
            <?php
//            ver($theme,d);
            if ($theme && in_array($theme, $arrTheme))
                echo '<link href="../library/bootstrap-3.0.0/css/bootstrap-theme-' . $theme . '.css" rel="stylesheet" media="screen">';
            ?>
            <!-- Bootstrap JS -->
            <script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
            
            <!-- Bootstrap DatePicker -->
<!--            <link rel='stylesheet' type='text/css' href='../library/bootstrap-datepicker/datepicker.css'/>
            <script src="../library/bootstrap-datepicker/bootstrap-datepicker.js" type="text/javascript" charset="ISO-8895-1"></script>-->
            

<!--<link href="http://netdna.bootstrapcdn.com/twitter-bootstrap/2.2.2/css/bootstrap-combined.min.css" rel="stylesheet">
<link rel="stylesheet" type="text/css" media="screen"  href="http://tarruda.github.com/bootstrap-datetimepicker/assets/css/bootstrap-datetimepicker.min.css">-->

            <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
            <!--[if lt IE 9]>
              <!--<script src="js/html5shiv.js"></script>-->
              <!--<script src="js/respond.min.js"></script>-->
            <![endif]-->

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

            <script language="JavaScript" src="../includes/funcoes.js"></script>

            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

        </head>
        <body>
<!--            <form id="form_system" method="post" action="/muda_sistema.php">
                <input type="hidden" name="sisid" id="sisid" value="" />
            </form>-->
            <form id="form_theme" method="post" action="">
                <input type="hidden" name="theme" id="theme" value="" />
            </form>
            <script lang="javascript">
                function changeSystem(system)
                {
//                    alert(system);
                    location.href = "../muda_sistema.php?sisid=" + system;
//                    $('#sisid').val(system);
//                    $('#form_system').submit();
                }
                function changeTheme(theme)
                {
                    $('#theme').val(theme);
                    $('#form_theme').submit();
                }
                
//                function simularUsuario(){
//                        var horizontal = 550;	
//                        var vertical   = 320;
//
//                        var res_ver = screen.height;
//                        var res_hor = screen.width;
//
//                        var pos_ver_fin = (parseInt(res_ver) - parseInt(vertical) )/2;
//                        var pos_hor_fin = (parseInt(res_hor) - parseInt(horizontal) )/2;
//
//                        //displayMessage('../includes/simularUsuario.php');
//                        return window.open("../includes/simularUsuario.php","Simular Usuário","width="+horizontal+",height="+vertical+",top="+pos_ver_fin+",left="+pos_hor_fin);
//
//                        //window.open("../includes/simularUsuario.php", "PTA","menubar=no,toolbar=no,scrollbars=no,resizable=no,left=20,top=20,width=560,height=345");
//                        //displayMessage('../includes/simularUsuario.php');
//                }
            </script>
            <div class="navbar navbar-default navbar-fixed-top">
                <!-- BOOTSTRAP BARRA BRASIL -->                
                <div class="rowbrasil">
                    <div id="barra-brasil">   
                        <div class="barra">
                            <ul>
                                <a title="Acesso à informação" href="http://www.acessoainformacao.gov.br">
                                    <li class="ai">
                                        www.sic.gov.br
                                    </li>
                                </a>
                                <a title="Portal de Estado do Brasil"  href="http://www.brasil.gov.br">
                                    <li class="brasilgov">
                                        www.brasil.gov.br
                                    </li>        
                                </a>
                            </ul>
                        </div>
                    </div>
                </div>
                <!-- /BOOTSTRAP BARRA BRASIL -->                
                <!--<div class="container">-->
                <div class="navbar-header">
                    <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                    </button>
                    <ul class="nav">
                        <li>
                            <a class="navbar-brand" href="#" onclick="javascript:changeSystem(<?php echo $_SESSION['sisid'] ?>)">
                                <!--SiMEC-->
                                <img width="100px" src="../includes/layout/planeta/img/logo.png">
                            </a>
                            <div class="navbar-brand" style="padding: 10px 0px 10px 0px;">
                            
                                <!--<span class="label label-default" style="font-size: 13px; font-family: Arial; font-weight: normal">Módulo </span> &nbsp;-->
                            <!--Módulo-->
                            <select data-placeholder="Escolha um módulo do sistema..." class="chosen-select" style="width:200px;" tabindex="2" onchange="javascipt:changeSystem(this.value);">
                                <option value="">Escolha um módulo do sistema...</option>
                                <?php foreach ($sistemas as $sistema): ?>
                                    <option <?php if($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                <?php endforeach; ?>
                            </select>
                            </div>
                        </li>
                    </ul>
                </div>
                <div class="collapse navbar-collapse">
                    <ul class="nav navbar-nav">
                        <?php echo $menus ?>
                    </ul>
                    <ul class="nav nav navbar-right navbar-btn">
                        <li class="nav-collapse">
                            <div class="btn-group">
                                <button class="btn btn-sm btn-<?php echo ($theme != 'default') ? 'default' : 'primary' ?>">
                                    <i class="glyphicon glyphicon-user"></i>
                                    <?php echo $_SESSION['usunome'] ?>
                                </button>
                                <button class="btn btn-sm btn-<?php echo ($theme != 'default') ? 'default' : 'primary' ?> dropdown-toggle" data-toggle="dropdown">
                                    <span class="caret"></span>
                                </button>
                                <ul class="dropdown-menu">
<!--                                    <li>
                                        <a href="#" onclick="javascript:simularUsuario();">
                                            <i class="glyphicon glyphicon-user"></i>
                                            Simular usuário
                                        </a>
                                    </li>-->
<!--                                    <li>
                                        <a data-toggle="modal" href="#myModal">
                                            <i class="glyphicon glyphicon-refresh"></i>
                                            Trocar de modulo
                                        </a>
                                    </li>-->
<!--                                    <li>
                                        <a href="#">
                                            <i class="glyphicon glyphicon-wrench"></i>
                                            Configurar
                                        </a>
                                    </li>-->
                                    <li>
                                        <a href="/logout.php">
                                            <i class="glyphicon glyphicon-off"></i>
                                            Sair
                                        </a>
                                    </li>
                                    <li role="presentation" class="divider"></li>
                                    <li role="presentation" class="dropdown-header">Temas</li>
                                    <li>
                                        <?php foreach($arrTheme as $theme): ?>
                                        <a href="#" onclick="javascript:changeTheme('<?php echo $theme ?>');">
                                            <!--<i class="glyphicon glyphicon-th-large"></i>-->
                                            <?php echo ucfirst($theme) ?>
                                        </a>
                                        <?php endforeach ?>
                                    </li>
                                </ul>
                            </div>
                        </li>
                    </ul>
                </div><!--/.nav-collapse -->
                <!--</div>-->
            </div>
            <br />
            <br />
            <br />
            <br />
            <br />
            <br />
<!--            <br />
            <br />-->
            <!-- Subhead  ================================================== -->
            <!--<header class="jumbotron subhead"></header><div class="container"></div>-->
            <div  style="padding: 10px; min-height: 570px;">
                <table border="0" width="98%" id="main" cellpadding="0" cellspacing="0" style="min-width: 780px;margin-left:7px;">
                    <tr>
                        <td align="left" valign="top" colspan="2" >
                            <form id="setperfil" name="setperfil" action=<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>&acao=C" method="post">
                                <table width="100%" border="0" cellpadding="2" cellspacing="0" class="notprint" >
                                    <tr>
                                        <td align="center">
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan=4 >
                                            teste 1

                                            fim teste


                                            <table class="tbl_conteudo" width="100%" border="0" cellpadding="0" cellspacing="0">
                                                <tr>
                                                    <td valign="top"  style="padding-bottom:15px;" >