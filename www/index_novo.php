<?php

// Escolhendo o tema do sistema.
if (isset($_GET['theme'])) {
    $theme = $_GET['theme'];
    setcookie("theme", $theme , time()+60*60*24*30, "/");
} else if (isset($_COOKIE["theme"])) {
    $theme = $_COOKIE["theme"];
} else {
    $theme = '';
}

$arrTheme = array ( 'ameliaa' , 'cerulean' , 'cosmo' , 'cyborg' , 'flatly' , 'journal' , 'readable' , 'simplex' , 'slate' , 'spacelab' , 'united' );

?>
<!DOCTYPE html>

<html>
    <head>
        <title>SIMEC - Sistema Integrado de Monitoramento ExecuÃ§Ã£o e Controle</title>
        <meta http-equiv='Content-Type' content='text/html; charset=iso-8859-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <!-- jQuery JS -->
        <script src="library/jquery/jquery-1.10.2.js" type="text/javascript" charset="iso-8859-1"></script>
        
        <!-- Bootstrap CSS -->
        <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
        <?php 
            if($theme && in_array($theme , $arrTheme))
                    echo '<link href="library/bootstrap-3.0.0/css/bootstrap-theme-' . $theme . '.css" rel="stylesheet" media="screen">';
        ?>
        <!--<link href="css/bootstrap-theme.css" rel="stylesheet" media="screen">-->
        <!--<link href="css/bootstrap-theme.min.css" rel="stylesheet" media="screen">-->
        <!-- Bootstrap JS -->
        <!--<script src="library/bootstrap-3.0.0/js/bootstrap.js" type="text/javascript" charset="utf-8"></script>-->
        <script src="library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="iso-8859-1"></script>
        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>
          <script src="js/html5shiv.js"></script>
          <script src="js/respond.min.js"></script>
        <![endif]-->

        <!-- Chosen CSS -->
        <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <style type="text/css" media="all">
            /* fix rtl for demo */
            .chosen-rtl .chosen-drop { left: -9000px; }
        </style>
        
        <!-- Custom CSS -->
        <link href="library/simec/css/custom.css" rel="stylesheet" media="screen">
        <link href="library/simec/css/css_reset.css" rel="stylesheet">
        <link href="library/simec/css/barra_brasil.css" rel="stylesheet">
        
    </head>
    <!DOCTYPE html>

    <body>
        <div class="navbar navbar-<?php echo ($theme != 'default')? 'default' : 'inverse' ?> navbar-fixed-top">
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
                        <a class="navbar-brand" href="#">
                            SiMEC 
                            <!--<span class="label">Esplanada Sustentável</span>-->
                            <select data-placeholder="Escolha um módulo do sistema..." class="chosen-select" style="width:200px;" tabindex="2">
                                <option value="Esplanada sustentável">Esplanada Sustentável</option>
                                <option value="PAR">PAR</option>
                                <option value="Pacto">Pacto</option>
                                <option value="Gerência de Projetos">Gerência de Projetos</option>
                                <option value="Mais Cultura">Mais Cultura</option>
                            </select>
                        </a>
                    </li>
                </ul>
            </div>
            <div class="collapse navbar-collapse">
                <ul class="nav navbar-nav">
                    <li class="dropdown">
                        <a class="dropdown-toggle" href="#" data-toggle="dropdown">
                            <i class="glyphicon glyphicon-home"></i> 
                            Principal
                            <b class="caret"></b>
                        </a>
                        <ul class="dropdown-menu">
                            <li class="dropdown-submenu">
                                <a href="#" tabindex="-1">Contratos</a>
                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="#">Submenu</a>
                                    </li>
                                    <li>
                                        <a href="#">Submenu</a>
                                    </li>
                                    <li class="dropdown-submenu">
                                        <a href="#" tabindex="-1">Contratos</a>
                                        <ul class="dropdown-menu">
                                            <li>
                                                <a href="#">Submenu</a>
                                            </li>
                                            <li>
                                                <a href="#">Submenu</a>
                                            </li>
                                            <li class="dropdown-submenu">
                                                <a href="#" tabindex="-1">Contratos</a>
                                                <ul class="dropdown-menu">
                                                    <li>
                                                        <a href="#">Submenu</a>
                                                    </li>
                                                    <li>
                                                        <a href="#">Submenu</a>
                                                    </li>
                                                    <li class="dropdown-submenu">
                                                        <a href="#" tabindex="-1">Contratos</a>
                                                        <ul class="dropdown-menu">
                                                            <li>
                                                                <a href="#">Submenu</a>
                                                            </li>
                                                            <li>
                                                                <a href="#">Submenu</a>
                                                            </li>
                                                        </ul>
                                                    </li>
                                                </ul>
                                            </li>
                                        </ul>
                                    </li>
                                </ul>
                            </li>
                            <li class="">
                                <a href="./getting-started.html">Plano de ação</a>
                            </li>
                            <li class="dropdown-submenu">
                                <a href="./getting-started.html">Ação</a>
                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="#">Submenu</a>
                                    </li>
                                    <li>
                                        <a href="#">Submenu</a>
                                    </li>
                                </ul>
                            </li>
                            <li class="dropdown-submenu">
                                <a href="./getting-started.html">Painel Gerêncial</a>
                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="#">Submenu</a>
                                    </li>
                                </ul>
                            </li>
                        </ul>
                    </li>
                    <li>
                        <a href="#">
                            <i class="glyphicon glyphicon-signal"></i>
                            Relatório
                        </a>
                    </li>
                    <li class="dropdown">
                        <a href="#" data-toggle="dropdown">
                            <i class="glyphicon glyphicon-cog"></i>
                            Sistema
                            <!--<b class="caret">Novo</b>-->
                            <b class="caret"></b>
                        </a>
                        <ul class="dropdown-menu">
                            <li class="dropdown-submenu">
                                <a href="#" tabindex="-1">Usuário</a>
                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="#">Gerênciar</a>
                                    </li>
                                    <li>
                                        <a href="#">Consultar</a>
                                    </li>
                                </ul>
                            </li>
                            <li class="">
                                <a href="./getting-started.html">Enviar email</a>
                            </li>
                            <li class="dropdown-submenu">
                                <a href="./getting-started.html">Perfil</a>
                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="#">Incluir perfil</a>
                                    </li>
                                    <li>
                                        <a href="#">Associar menus</a>
                                    </li>
                                </ul>
                            </li>
                            <li class="dropdown-submenu">
                                <a href="./getting-started.html">Menu</a>
                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="#">Administrar menu</a>
                                    </li>
                                    <li>
                                        <a href="#">Administrar abas</a>
                                    </li>
                                </ul>
                            </li>
                        </ul>
                    </li>
                </ul>
                <ul class="nav nav navbar-right navbar-btn">
                    <li class="nav-collapse">
                        <div class="btn-group">
                            <button class="btn btn-sm btn-<?php echo ($theme != 'default')? 'default' : 'primary' ?>">
                                <i class="glyphicon glyphicon-user"></i>
                                Ruy Junior Ferreira Silva
                            </button>
                            <button class="btn btn-sm btn-<?php echo ($theme != 'default')? 'default' : 'primary' ?> dropdown-toggle" data-toggle="dropdown">
                                <span class="caret"></span>
                            </button>
                            <ul class="dropdown-menu">
                                <li>
                                    <a href="#">
                                        <i class="glyphicon glyphicon-user"></i>
                                        Simular usuário
                                    </a>
                                </li>
                                <li>
                                    <a data-toggle="modal" href="#myModal">
                                        <i class="glyphicon glyphicon-refresh"></i>
                                        Trocar de modulo
                                    </a>
                                </li>
                                <li>
                                    <a href="#">
                                        <i class="glyphicon glyphicon-wrench"></i>
                                        Configurar
                                    </a>
                                </li>
                                <li>
                                    <a href="#">
                                        <i class="glyphicon glyphicon-off"></i>
                                        Sair
                                    </a>
                                </li>
                                <li role="presentation" class="divider"></li>
                                <li role="presentation" class="dropdown-header">Temas</li>
                                <li>
                                    <a href="index_novo.php?theme=default">
                                        <!--<i class="glyphicon glyphicon-th-large"></i>-->
                                        Default
                                    </a>
                                    <a href="index_novo.php?theme=ameliaa">
                                        <!--<i class="glyphicon glyphicon-th-large"></i>-->
                                        Amelia
                                    </a>
                                    <a href="index_novo.php?theme=cerulean">
                                        Cerulean
                                    </a>
                                    <a href="index_novo.php?theme=cosmo">
                                        Cosmo
                                    </a>
                                    <a href="index_novo.php?theme=cyborg">
                                        Cyborg
                                    </a>
                                    <a href="index_novo.php?theme=flatly">
                                        Flatly
                                    </a>
                                    <a href="index_novo.php?theme=journal">
                                        Journal
                                    </a>
                                    <a href="index_novo.php?theme=readable">
                                        Readable
                                    </a>
                                    <a href="index_novo.php?theme=simplex">
                                        Simplex
                                    </a>
                                    <a href="index_novo.php?theme=slate">
                                        Slate
                                    </a>
                                    <a href="index_novo.php?theme=spacelab">
                                        Spacelab
                                    </a>
                                    <a href="index_novo.php?theme=united">
                                        United
                                    </a>
                                </li>
                            </ul>
                        </div>
                    </li>
                </ul>

            </div><!--/.nav-collapse -->
            <!--</div>-->
        </div>
        <!-- Navbar ================================================== -->
        <!--        <div class="navbar navbar-inverse navbar-fixed-top">
                                            <div class="navbar-inner">
                                                <div class="container">
                                                    <ul class="nav nav-pills">
                                                        <li>
                                                            <a class="navbar-brand" href="#">
                                                                <img src="/img/favicon.png">
                                                                SIMEC 
                                                                <span class="label">Esplanada Sustentável</span>
                                                            </a>
                                                        </li>
                                                    </ul>
                                                    <ul class="nav pull-right">
                                                        <li class="divider-vertical"></li>
                                                        <li>
                                                            <div class="btn-group">
                                                                <button class="btn btn-danger">
                                                                    <i class="icon-user icon-white"></i>
                                                                    Login
                                                                </button>
                                                                <button class="btn btn-danger dropdown-toggle" data-toggle="dropdown">
                                                                    <span class="caret"></span>
                                                                </button>
                                                                <ul class="dropdown-menu">
                                                                    <li>
                                                                        <a href="#">Esqueci minha senha</a>
                                                                    </li>
                                                                    <li>
                                                                        <a href="#">Cadastro de novo usuário</a>
                                                                    </li>
                                                                    <li>
                                                                        <a href="#">Something else here</a>
                                                                    </li>
                                                                        <li class="divider"></li>
                                                                    <li>
                                                                        <a href="#">Separated link</a>
                                                                    </li>
                                                                </ul>
                                                            </div>
                                                        </li>
                                                    </ul>
                                                </div>
                                            </div>
                    <div class="navbar-inner">
                        <div class="container">
                            <ul class="nav nav-pills">
                                <li style="">
                                    <a class="navbar-brand" href="#">
                                        <img src="simec2013/img/logo-simec.png" style="height: 25px;" />
                                        SiMEC 
                                        <select data-placeholder="Escolha um módulo do sistema..." class="chosen-select" style="width:200px;" tabindex="2">
                                            <option value="Esplanada sustentável">Esplanada Sustentável</option>
                                            <option value="PAR">PAR</option>
                                            <option value="Pacto">Pacto</option>
                                            <option value="Gerência de Projetos">Gerência de Projetos</option>
                                            <option value="Mais Cultura">Mais Cultura</option>
                                        </select>
                                    </a>
                                    <span class="label">Esplanada Sustentável</span>
                                </li>
                                <li class="dropdown">
                                    <a href="#" data-toggle="dropdown">
                                        <i class="glyphicon glyphicon-home"></i> 
                                        Principal
                                    </a>
                                    <ul class="dropdown-menu">
                                        <li class="dropdown-submenu">
                                            <a href="#" tabindex="-1">Contratos</a>
                                            <ul class="dropdown-menu">
                                                <li>
                                                    <a href="#">Submenu</a>
                                                </li>
                                                <li>
                                                    <a href="#">Submenu</a>
                                                </li>
                                            </ul>
                                        </li>
                                        <li class="">
                                            <a href="./getting-started.html">Plano de ação</a>
                                        </li>
                                        <li class="dropdown-submenu">
                                            <a href="./getting-started.html">Ação</a>
                                            <ul class="dropdown-menu">
                                                <li>
                                                    <a href="#">Submenu</a>
                                                </li>
                                                <li>
                                                    <a href="#">Submenu</a>
                                                </li>
                                            </ul>
                                        </li>
                                        <li class="dropdown-submenu">
                                            <a href="./getting-started.html">Painel Gerêncial</a>
                                            <ul class="dropdown-menu">
                                                <li>
                                                    <a href="#">Submenu</a>
                                                </li>
                                            </ul>
                                        </li>
                                    </ul>
                                </li>
                                <li >
                                    <a href="#">
                                        <i class="icon-signal icon-white"></i>
                                        Relatório
                                    </a>
                                </li>
                                <li class="dropdown">
                                    <a href="#" data-toggle="dropdown">
                                        <i class="icon-cog icon-white"></i>
                                        Sistema
                                        <b class="caret">Novo</b>
                                    </a>
                                    <ul class="dropdown-menu">
                                        <li class="dropdown-submenu">
                                            <a href="#" tabindex="-1">Usuário</a>
                                            <ul class="dropdown-menu">
                                                <li>
                                                    <a href="#">Gerênciar</a>
                                                </li>
                                                <li>
                                                    <a href="#">Consultar</a>
                                                </li>
                                            </ul>
                                        </li>
                                        <li class="">
                                            <a href="./getting-started.html">Enviar email</a>
                                        </li>
                                        <li class="dropdown-submenu">
                                            <a href="./getting-started.html">Perfil</a>
                                            <ul class="dropdown-menu">
                                                <li>
                                                    <a href="#">Incluir perfil</a>
                                                </li>
                                                <li>
                                                    <a href="#">Associar menus</a>
                                                </li>
                                            </ul>
                                        </li>
                                        <li class="dropdown-submenu">
                                            <a href="./getting-started.html">Menu</a>
                                            <ul class="dropdown-menu">
                                                <li>
                                                    <a href="#">Administrar menu</a>
                                                </li>
                                                <li>
                                                    <a href="#">Administrar abas</a>
                                                </li>
                                            </ul>
                                        </li>
                                    </ul>
                                </li>
                            </ul>
                            <ul class="nav pull-right">
                                <li class="divider-vertical"></li>
                                <li>
                                                                                            <div class="-group" data-toggle="buttons-radio">
                                                                                                <button id="edit" class="active btn btn-sm btn-primary" type="button">
                                                                                                    <i class="icon-edit icon-white"></i>
                                                                                                    Edit
                                                                                                </button>
                                                                                                <button id="devpreview" class="btn btn-sm btn-primary" type="button">
                                                                                                    <i class="icon-eye-close icon-white"></i>
                                                                                                    Developer view
                                                                                                </button>
                                                                                                <button id="sourcepreview" class="btn btn-sm btn-primary" type="button">
                                                                                                    <i class="icon-eye-open icon-white"></i>
                                                                                                    Preview
                                                                                                </button>
                                                                                            </div>
                                                                                            <div class="btn-group">
                                                                                                <button class="btn btn-sm btn-primary" data-toggle="modal" role="button" rel="/build/downloadModal" data-target="#downloadModal" type="button">
                                                                                                    <i class="icon-chevron-down icon-white"></i>
                                                                                                    Download
                                                                                                </button>
                                                                                                <button class="btn btn-sm btn-primary" data-target="#shareModal" data-toggle="modal" role="button" href="/share/index">
                                                                                                    <i class="icon-share icon-white"></i>
                                                                                                    Share or Save
                                                                                                </button>
                                                                                                <button id="clear" class="btn btn-sm btn-primary" href="#clear">
                                                                                                    <i class="icon-trash icon-white"></i>
                                                                                                    Clear
                                                                                                </button>
                                                                                            </div>
                                                                                            <div class="btn-group" data-toggle="buttons-radio">
                                                                                                <button id="feedback" class="btn btn-sm btn-primary" data-target="#feedbackModal" data-toggle="modal" role="button" href="/feedbacks/index">
                                                                                                    <i class="icon-comment icon-white"></i>
                                                                                                    Mensagens
                                                                                                </button>
                                                                                            </div>
                                    <div class="btn-group">
        
                                        <button class="btn btn-primary">
                                            <i class="icon-user icon-white"></i>
                                            Ruy Junior Ferreira Silva
                                        </button>
                                        <button class="btn btn-primary dropdown-toggle" data-toggle="dropdown">
                                            <span class="caret"></span>
                                        </button>
                                        <ul class="dropdown-menu">
                                            <li>
                                                <a href="#">
                                                    <i class="icon-user icon-black"></i>
                                                    Simular usuário
                                                </a>
                                            </li>
                                            <li>
                                                <a data-toggle="modal" href="#myModal">
                                                    <i class="icon-refresh icon-black"></i>
                                                    Trocar de modulo
                                                </a>
                                            </li>
                                            <li>
                                                <a href="#">
                                                    <i class="icon-wrench icon-black"></i>
                                                    Configurar
                                                </a>
                                            </li>
                                            <li>
                                                <a href="#">
                                                    <i class="icon-off icon-black"></i>
                                                    Sair
                                                </a>
                                            </li>
                                        </ul>
                                                                                                        <button class="btn btn-danger">
                                                                                                            <i class="icon-user icon-white"></i>
                                                                                                            Login
                                                                                                        </button>
                                                                                                        <button class="btn btn-danger dropdown-toggle" data-toggle="dropdown">
                                                                                                            <span class="caret"></span>
                                                                                                        </button>
                                                                                                        <ul class="dropdown-menu">
                                                                                                            <li>
                                                                                                                <a href="#">Esqueci minha senha</a>
                                                                                                            </li>
                                                                                                            <li>
                                                                                                                <a href="#">Cadastro de novo usuário</a>
                                                                                                            </li>
                                                                                                            <li>
                                                                                                                <a href="#">Something else here</a>
                                                                                                            </li>
                                                                                                                <li class="divider"></li>
                                                                                                            <li>
                                                                                                                <a href="#">Separated link</a>
                                                                                                            </li>
                                                                                                        </ul>
                                    </div>
                                </li>
                            </ul>
                        </div>
                    </div>
                </div>-->
        <br />
        <br />
        <br />
        <br />
        <br />
        <!-- Subhead  ================================================== -->
        <!--<header class="jumbotron subhead"></header><div class="container"></div>-->

        <script src="http://code.jquery.com/jquery.js"></script>
        <script src="js/bootstrap.min.js"></script>

        <div class="container">
            <!--            <div class="row">
                            <div class="col-md-12">
                                
                            </div>
                        </div>-->
            <!--            <div class="row">
                            <div class="col-md-12">
                                <div class="well">
                                    <form class="bs-example">
                                        <div class="form-group">
                                          <label class="control-label" for="focusedInput">Focused input</label>
                                          <input class="form-control" id="focusedInput" type="text" value="This is focused...">
                                        </div>
                                        <div class="form-group">
                                          <label class="control-label" for="disabledInput">Disabled input</label>
                                          <input class="form-control" id="disabledInput" type="text" placeholder="Disabled input here..." disabled="">
                                        </div>
                                        <div class="form-group  has-warning">
                                          <label class="control-label" for="inputWarning">Input warning</label>
                                          <input type="text" class="form-control" id="inputWarning">
                                        </div>
                                        <div class="form-group has-error">
                                          <label class="control-label" for="inputError">Input error</label>
                                          <input type="text" class="form-control" id="inputError">
                                        </div>
                                        <div class="form-group has-success">
                                          <label class="control-label" for="inputSuccess">Input success</label>
                                          <input type="text" class="form-control" id="inputSuccess">
                                        </div>
                                        <div class="form-group">
                                          <label class="control-label" for="inputLarge">Large input</label>
                                          <input class="form-control input-lg" type="text" id="inputLarge">
                                        </div>
                                        <div class="form-group">
                                          <label class="control-label" for="inputDefault">Default input</label>
                                          <input type="text" class="form-control" id="inputDefault">
                                        </div>
                                        <div class="form-group">
                                          <label class="control-label" for="inputSmall">Small input</label>
                                          <input class="form-control input-sm" type="text" id="inputSmall">
                                        </div>
                                        <div class="form-group">
                                          <label class="control-label">Input addons</label>
                                          <div class="input-group">
                                            <span class="input-group-addon">$</span>
                                            <input type="text" class="form-control">
                                            <span class="input-group-btn">
                                              <button class="btn btn-default" type="button">Button</button>
                                            </span>
                                          </div>
                                        </div>
                                  </form>
                                </div>
                            </div>
                        </div>-->
            <!-- Forms ===================================================== -->
            <div class="row">
                <div class="col-md-12">
                    <div class="page-header">
                        <h1 id="forms">Usuário</h1>
                    </div>
                    <div class="col-lg-12">
                        <div class="bs-example">
                            <div class="alert alert-dismissable alert-warning">
                                <button class="close" data-dismiss="alert" type="button">×</button>
                                <h4>Aviso!</h4>
                                <p>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-12">
                        <div class="alert alert-dismissable alert-danger">
                            <button class="close" data-dismiss="alert" type="button">×</button>
                            <strong>Erro!</strong>
                            <a class="alert-link" href="#" >Dados incorretos,</a>
                            preencha todos os campos e submeta o formulário novamente.
                        </div>
                    </div>
                    <div class="col-lg-12">
                        <div class="alert alert-dismissable alert-success">
                            <button class="close" data-dismiss="alert" type="button">×</button>
                            <strong>Sucesso!</strong>
                            Dados salvos com sucesso!
                            <a class="alert-link" href="#"></a>
                        </div>
                    </div>
                    <div class="col-lg-12">
                        <div class="alert alert-dismissable alert-info">
                            <button class="close" data-dismiss="alert" type="button">×</button>
                            <strong>Atenção!</strong>
                            Este
                            <a class="alert-link" href="#"> alerta precisa de sua atenção</a>, mas não é tão importante assim.
                        </div>
                    </div>
                    <div class="well">
                        <form>
                            <fieldset>
                                <legend>Formulário de cadastro do usuário</legend>
                                <div class="form-group">
                                </div>
                                <div class="form-group has-success">
                                    <label class="control-label" for="inputSuccess">Input com sucesso</label>
                                    <input type="text" class="form-control" id="inputSuccess">
                                </div>
                                <div class="form-group has-warning">
                                    <label class="control-label" for="inputWarning">Input com atenção</label>
                                    <input type="text" class="form-control" id="inputWarning">
                                </div>
                                <div class="form-group has-error">
                                    <label class="control-label" for="inputError">Input com erro</label>
                                    <input type="text" class="form-control" id="inputError">
                                </div>
                                <div class="form-group">
                                    <label for="exampleInputEmail1">Email</label>
                                    <input type="email" class="form-control" id="exampleInputEmail1" placeholder="Coloque aqui seu email">
                                </div>
                                <div class="form-group">
                                    <label for="exampleInputPassword1">Senha</label>
                                    <input type="password" class="form-control" id="exampleInputPassword1" placeholder="Coloque aqui sua senha">
                                </div>
                                <div class="form-group">
                                    <label for="exampleInputPassword1">Selecione um número</label>
                                    <select class="form-control" id="select">
                                        <option>1</option>
                                        <option>2</option>
                                        <option>3</option>
                                        <option>4</option>
                                        <option>5</option>
                                    </select>

                                </div>
                                <div class="form-group ">
                                    <label for="exampleInputPassword1">Selecione um número</label>
                                    <select multiple="" class="form-control">
                                        <option>1</option>
                                        <option>2</option>
                                        <option>3</option>
                                        <option>4</option>
                                        <option>5</option>
                                    </select>
                                </div>
                                <div class="form-group">
                                    <label for="exampleInputFile">Arquivo</label>
                                    <input type="file" id="exampleInputFile">
                                    <p class="help-block">Exemplo de texto de ajuda.</p>
                                </div>
                                <div class="clearfix"></div>
                                <div class="checkbox">
                                    <label>
                                        <input type="checkbox"> Receber email.
                                    </label>
                                </div>
                                <div class="text-center">

                                    <button type="submit" class="btn btn-default">Salvar</button>
                                </div>
                            </fieldset>
                        </form>
                    </div>
                    <!-- Forms
      ================================================== -->
                    <div class="bs-docs-section">
                        <div class="row">
                            <div class="col-lg-12">
                                <div class="page-header">
                                    <h1 id="forms">Usuário</h1>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-lg-6">
                                <div class="well">
                                    <form class="bs-example form-horizontal">
                                        <fieldset>
                                            <legend>Formulário</legend>
                                            <div class="form-group">
                                                <label for="inputEmail" class="col-lg-2 control-label">Email</label>
                                                <div class="col-lg-10">
                                                    <input type="text" class="form-control" id="inputEmail" placeholder="Email">
                                                </div>
                                            </div>
                                            <div class="form-group">
                                                <label for="inputPassword" class="col-lg-2 control-label">Password</label>
                                                <div class="col-lg-10">
                                                    <input type="password" class="form-control" id="inputPassword" placeholder="Password">
                                                    <div class="checkbox">
                                                        <label>
                                                            <input type="checkbox"> Checkbox
                                                        </label>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="form-group">
                                                <label for="textArea" class="col-lg-2 control-label">Textarea</label>
                                                <div class="col-lg-10">
                                                    <textarea class="form-control" rows="3" id="textArea"></textarea>
                                                    <span class="help-block">A longer block of help text that breaks onto a new line and may extend beyond one line.</span>
                                                </div>
                                            </div>
                                            <div class="form-group">
                                                <label class="col-lg-2 control-label">Radios</label>
                                                <div class="col-lg-10">
                                                    <div class="radio">
                                                        <label>
                                                            <input type="radio" name="optionsRadios" id="optionsRadios1" value="option1" checked="">
                                                            Option one is this
                                                        </label>
                                                    </div>
                                                    <div class="radio">
                                                        <label>
                                                            <input type="radio" name="optionsRadios" id="optionsRadios2" value="option2">
                                                            Option two can be something else
                                                        </label>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="form-group">
                                                <label for="select" class="col-lg-2 control-label">Selects</label>
                                                <div class="col-lg-10">
                                                    <select class="form-control" id="select">
                                                        <option>1</option>
                                                        <option>2</option>
                                                        <option>3</option>
                                                        <option>4</option>
                                                        <option>5</option>
                                                    </select>
                                                    <br>
                                                    <select multiple="" class="form-control">
                                                        <option>1</option>
                                                        <option>2</option>
                                                        <option>3</option>
                                                        <option>4</option>
                                                        <option>5</option>
                                                    </select>
                                                </div>
                                            </div>
                                            <div class="form-group">
                                                <div class="col-lg-10 col-lg-offset-2">
                                                    <button class="btn btn-default">Cancel</button> 
                                                    <button type="submit" class="btn btn-primary">Submit</button> 
                                                </div>
                                            </div>
                                        </fieldset>
                                    </form>
                                </div>
                            </div>
                            <div class="col-lg-4 col-lg-offset-1">

                                <form class="bs-example">
                                    <div class="form-group">
                                        <label class="control-label" for="focusedInput">Focused input</label>
                                        <input class="form-control" id="focusedInput" type="text" value="This is focused...">
                                    </div>
                                    <div class="form-group">
                                        <label class="control-label" for="disabledInput">Disabled input</label>
                                        <input class="form-control" id="disabledInput" type="text" placeholder="Disabled input here..." disabled="">
                                    </div>
                                    <div class="form-group has-warning">
                                        <label class="control-label" for="inputWarning">Input warning</label>
                                        <input type="text" class="form-control" id="inputWarning">
                                    </div>
                                    <div class="form-group has-error">
                                        <label class="control-label" for="inputError">Input error</label>
                                        <input type="text" class="form-control" id="inputError">
                                    </div>
                                    <div class="form-group has-success">
                                        <label class="control-label" for="inputSuccess">Input success</label>
                                        <input type="text" class="form-control" id="inputSuccess">
                                    </div>
                                    <div class="form-group">
                                        <label class="control-label" for="inputLarge">Large input</label>
                                        <input class="form-control input-lg" type="text" id="inputLarge">
                                    </div>
                                    <div class="form-group">
                                        <label class="control-label" for="inputDefault">Default input</label>
                                        <input type="text" class="form-control" id="inputDefault">
                                    </div>
                                    <div class="form-group">
                                        <label class="control-label" for="inputSmall">Small input</label>
                                        <input class="form-control input-sm" type="text" id="inputSmall">
                                    </div>
                                    <div class="form-group">
                                        <label class="control-label">Input addons</label>
                                        <div class="input-group">
                                            <span class="input-group-addon">$</span>
                                            <input type="text" class="form-control">
                                            <span class="input-group-btn">
                                                <button class="btn btn-default" type="button">Button</button>
                                            </span>
                                        </div>
                                    </div>
                                </form>

                            </div>
                        </div>
                    </div>
                </div>
            </div>



            <!--            <div class="row">
                            <div class="col-md-12">
                                <form>
                                    <fieldset>
                                        <legend>Formulário de cadastro de usuário</legend>
                                        <label>Nome</label>
                                        <input type="text" />
                                        <span class="help-block">Exemplo: João</span> 
                                        <label>Sobre nome</label>
                                        <input class="" type="text" />
                                        <span class="help-block">Exemplo: Costa da Silva.</span> 
                                        <label class="checkbox">
                                            <input type="checkbox" /> Receber informações por e-mail?
                                        </label>
                                        <br />
                                        <button type="submit" class="btn">Submit</button>
                                    </fieldset>
                                </form>
            
                                <button class="btn btn-block" type="button">Button</button>
                            </div>
                        </div>-->
            <!--            <div class="row">
                            <div class="col-md-12">
                                <table class="table">
                                    <thead>
                                        <tr>
                                            <th>
                                                #
                                            </th>
                                            <th>
                                                Product
                                            </th>
                                            <th>
                                                Payment Taken
                                            </th>
                                            <th>
                                                Status
                                            </th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td>
                                                1
                                            </td>
                                            <td>
                                                TB - Monthly
                                            </td>
                                            <td>
                                                01/04/2012
                                            </td>
                                            <td>
                                                Default
                                            </td>
                                        </tr>
                                        <tr class="success">
                                            <td>
                                                1
                                            </td>
                                            <td>
                                                TB - Monthly
                                            </td>
                                            <td>
                                                01/04/2012
                                            </td>
                                            <td>
                                                Approved
                                            </td>
                                        </tr>
                                        <tr class="error">
                                            <td>
                                                2
                                            </td>
                                            <td>
                                                TB - Monthly
                                            </td>
                                            <td>
                                                02/04/2012
                                            </td>
                                            <td>
                                                Declined
                                            </td>
                                        </tr>
                                        <tr class="warning">
                                            <td>
                                                3
                                            </td>
                                            <td>
                                                TB - Monthly
                                            </td>
                                            <td>
                                                03/04/2012
                                            </td>
                                            <td>
                                                Pending
                                            </td>
                                        </tr>
                                        <tr class="info">
                                            <td>
                                                4
                                            </td>
                                            <td>
                                                TB - Monthly
                                            </td>
                                            <td>
                                                04/04/2012
                                            </td>
                                            <td>
                                                Call in to confirm
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
            
                                <button class="btn btn-block" type="button">Button</button>
                            </div>
                        </div>-->
        </div>
        <!-- Modal -->
        <div class="row">
            <div class="col-md-12">

                <div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-dialog">
                        <div class="modal-content text-center">
                            <div class="modal-header">
                                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                                <h4 class="modal-title">Selecione um módulo do SiMEC.</h4>
                            </div>
                            <div class="modal-body">
                                <div>
                                    <form class="form-search">
                                        <div class="form-group">
                                            <div class="input-group">
                                                <span class="input-group-addon glyphicon glyphicon-search"></span>
                                                <input type="text" class="form-control">
                                                <span class="input-group-btn">
                                                    <button  placeholder="Digite aqui o nome do módulo" class="btn btn-default" type="button" >Procurar</button>
                                                </span>
                                            </div>
                                        </div>
                                    </form>
                                </div>
                                <!-- Tables
      ================================================== -->
                                <div class="bs-docs-section">
                                    <div class="row">
                                        <div class="col-lg-12">
                                            <div class="page-header">
                                                <!--<h1 id="tables">Listagem dos módulos</h1>-->
                                            </div>
                                            <div class="bs-example">
                                                <table class="table table-striped table-bordered table-hover">
                                                    <thead>
                                                        <tr >
                                                            <th>#</th>
                                                            <th class="text-center">Nome</th>
                                                            <th class="text-center">Data de criação</th>
                                                            <th class="text-center">Status</th>
                                                        </tr>
                                                    </thead>
                                                    <tbody>
                                                        <tr>
                                                            <td>
                                                                1
                                                            </td>
                                                            <td>
                                                                Esplanada Sustentável
                                                            </td>
                                                            <td>
                                                                01/04/2012
                                                            </td>
                                                            <td>
                                                                Selecionado
                                                            </td>
                                                        </tr>
                                                        <tr class="success" data-dismiss="modal" style="cursor: pointer;">
                                                            <td>
                                                                1
                                                            </td>
                                                            <td>
                                                                PAR
                                                            </td>
                                                            <td>
                                                                01/04/2012
                                                            </td>
                                                            <td>
                                                                Ativo
                                                            </td>
                                                        </tr>
                                                        <tr class="danger" data-dismiss="modal" style="cursor: pointer;">
                                                            <td>
                                                                2
                                                            </td>
                                                            <td>
                                                                Pacto
                                                            </td>
                                                            <td>
                                                                02/04/2012
                                                            </td>
                                                            <td>
                                                                Sobrecarregado
                                                            </td>
                                                        </tr>
                                                        <tr class="warning" data-dismiss="modal" style="cursor: pointer;">
                                                            <td>
                                                                3
                                                            </td>
                                                            <td>
                                                                Gerência de Projetos
                                                            </td>
                                                            <td>
                                                                03/04/2012
                                                            </td>
                                                            <td>
                                                                Muito acesso
                                                            </td>
                                                        </tr>
                                                        <tr class="active" data-dismiss="modal" style="cursor: pointer;">
                                                            <td>
                                                                4
                                                            </td>
                                                            <td>
                                                                Mais Cultura
                                                            </td>
                                                            <td>
                                                                04/04/2012
                                                            </td>
                                                            <td>
                                                                Estável
                                                            </td>
                                                        </tr>
<!--                                                        <tr>
                                                            <td>1</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>
                                                        <tr>
                                                            <td>2</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>
                                                        <tr>
                                                            <td>3</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>
                                                        <tr class="success">
                                                            <td>4</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>
                                                        <tr class="danger">
                                                            <td>5</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>
                                                        <tr class="warning">
                                                            <td>6</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>
                                                        <tr class="active">
                                                            <td>7</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                            <td>Column content</td>
                                                        </tr>-->
                                                    </tbody>
                                                </table>
                                            </div><!-- /example -->
                                        </div>
                                    </div>
                                </div>
                                <ul class="pagination">
                                    <li class="disabled"><a href="#">&laquo;</a></li>
                                    <li class="active"><a href="#">1 <span class="sr-only">(current)</span></a></li>
                                    <li><a href="#">2</a></li>
                                    <li><a href="#">3</a></li>
                                    <li><a href="#">4</a></li>
                                    <li><a href="#">5</a></li>
                                    <li><a href="#">&raquo;</a></li>
                                </ul>

                            </div>
                            <div class="modal-footer">
                                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                                <!--<button type="button" class="btn btn-primary">Save changes</button>-->
                            </div>
                        </div><!-- /.modal-content -->
                    </div><!-- /.modal-dialog -->
                </div><!-- /.modal -->

            </div>
        </div>
        <br />
        <br />
        <div class="row cont" style="background-color: #000;">
            <br />
            <div class="row">
                <div class="col-md-4">
                    <ul>
                        <li>
                            Data: 27/08/2013 - 14:24:21 / Último acesso (27/08/2013) - Usuários On-Line 
                        </li>
                    </ul>
                </div>
                <div class="col-md-3 text-center">
                    <ul>
                        <li>
                            Visualizar Regras
                        </li>
                        <!--                        <li>
                                                    Consectetur adipiscing elit
                                                </li>
                                                <li>
                                                    Integer molestie lorem at massa
                                                </li>
                                                <li>
                                                    Facilisis in pretium nisl aliquet
                                                </li>
                                                <li>
                                                    Nulla volutpat aliquam velit
                                                </li>
                                                <li>
                                                    Faucibus porta lacus fringilla vel
                                                </li>
                                                <li>
                                                    Aenean sit amet erat nunc
                                                </li>
                                                <li>
                                                    Eget porttitor lorem
                                                </li>-->
                    </ul>
                </div>
                <!--                <div class="col-md-3">
                                    <ol>
                                        <li>
                                            Lorem ipsum dolor sit amet
                                        </li>
                                        <li>
                                            Consectetur adipiscing elit
                                        </li>
                                        <li>
                                            Integer molestie lorem at massa
                                        </li>
                                        <li>
                                            Facilisis in pretium nisl aliquet
                                        </li>
                                        <li>
                                            Nulla volutpat aliquam velit
                                        </li>
                                        <li>
                                            Faucibus porta lacus fringilla vel
                                        </li>
                                        <li>
                                            Aenean sit amet erat nunc
                                        </li>
                                        <li>
                                            Eget porttitor lorem
                                        </li>
                                    </ol>
                                </div>-->
                <div class="col-md-4 text-right">
                    <address>
                        <!--                        <strong>Twitter, Inc.</strong>
                                                <br /> 795 Folsom Ave, Suite 600
                                                <br /> San Francisco, CA 94107
                                                <br /> <abbr title="Phone">P:</abbr> (123) 456-7890-->
                        SIMEC - Fale Conosco Manual	| Tx.: 0,2015s / 0,34 
                    </address>
                </div>
            </div>
        </div>
        <script src="library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <script type="text/javascript">
            var config = {
                '.chosen-select': {},
                '.chosen-select-deselect': {allow_single_deselect: true},
                '.chosen-select-no-single': {disable_search_threshold: 10},
                '.chosen-select-no-results': {no_results_text: 'Oops, nothing found!'},
                '.chosen-select-width': {width: "95%"}
            }
            for (var selector in config) {
                $(selector).chosen(config[selector]);
            }
        </script> 
    </body>
</html>
