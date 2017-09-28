<?php

// Verifica se a sessao expirou.
if( ! isset($_SESSION['usucpf']) )
{
    session_unset();
    $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
    header('Location: login.php');
    exit();
}

include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';

if ($_POST['carregarSituacaoPainel']):

    if ($_POST['tpdid']):

        $sql = "select esdid as codigo, esddsc as descricao
        from workflow.estadodocumento
        where tpdid = {$_POST['tpdid']}
        order by descricao
        ";
        $estadosDocumento = $db->carregar($sql);
        ?>
        <select id="estado-documento-situacao-workflow" name="esdid[]" class="form-control sel_chosen chosen" multiple="multiple" data-placeholder="Selecione">
            <option value=""></option>
            <?php if ($estadosDocumento): ?>
                <?php foreach ($estadosDocumento as $estadoDocumento): ?>
                    <option value="<?php echo $estadoDocumento['codigo'] ?>"><?php echo $estadoDocumento['descricao'] ?></option>
                <?php endforeach; ?>
            <?php endif ?>
        </select>
    <?php else : ?>
        <select id="" name="esdid[]" class="form-control sel_chosen chosen" multiple="multiple" data-placeholder="Selecione">
        </select>
    <?php  endif; exit; ?>
<?php elseif ($_POST['gerarGraficoPainel']): ?>
    <?php include_once APPRAIZ . "includes/library/simec/Grafico.php"; ?>
    <div  style="padding: 15px; ">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                    <?php
                    $dt_inicio = $_REQUEST['dt_inicio'] ? formata_data_sql($_REQUEST['dt_inicio']) : date('Y') . '-01-01';
                    $dt_fim = $_REQUEST['dt_fim'] ? formata_data_sql($_REQUEST['dt_fim']) : date('Y-m-d');
                    $tpdid = $_POST['tpdid'];
                    $esdid = $_POST['esdid'];


                    switch ($_REQUEST['tipo_exibicao']) {
                        case('D'):
                            $formatoBusca = 'YYYYMMDD';
                            $formatoExibicao = 'DD/MM/YYYY';
                            break;
                        case('A'):
                            $formatoBusca = 'YYYY';
                            $formatoExibicao = 'YYYY';
                            break;
                        default:
                            $formatoBusca = 'YYYYMM';
                            $formatoExibicao = 'MM/YYYY';
                            break;
                    }

                    if ($esdid && is_array($esdid)) {
                        $esdid = 'and aed.esdiddestino in (' . implode(', ', array_filter($esdid)) . ')';
                    } else {
                        $esdid = '';
                    }

                    if ($tpdid == 44) {
                        // Se for fluxo PAR
                        $innerPar = 'inner join par.instrumentounidade iu on iu.docid = d.docid';
                    } else {
                        $innerPar = '';
                    }

                    $sql = "select count(*) as valor, to_char(h.htddata, '{$formatoBusca}') as data, to_char(h.htddata, '{$formatoExibicao}') as categoria, aed.esdiddestino, e.esddsc as descricao
                            from
                                workflow.historicodocumento h
                                inner join  workflow.documento d  on h.docid = d.docid
                                inner join workflow.acaoestadodoc aed on aed.aedid = h.aedid
                                inner join workflow.estadodocumento e on aed.esdiddestino = e.esdid
                                {$innerPar}
                            where h.htddata BETWEEN '{$dt_inicio} 00:00:00' and '{$dt_fim} 23:59:59'
                            and e.tpdid = {$tpdid}
                            and e.esdstatus = 'A'
                            and aed.aedstatus = 'A'
                            $esdid
                            $esdid
                            group by data, categoria, aed.esdiddestino, descricao
                            order by data, descricao";

                    $sql2 = "select count (d.docid) as valor, esd.esddsc  descricao
                                FROM workflow.documento d
                                inner join workflow.estadodocumento esd on esd.esdid = d.esdid
                              {$innerPar}
                                where esd.tpdid = {$tpdid}
                                group by esd.esddsc,esd.tpdid";

                    $grafico = new Grafico(NULL, false);
                    ?>
                    <table class="tabela">
                        <tr>
                            <td>
                                <?php $grafico->setTipo(Grafico::K_TIPO_LINHA)->setWidth('50%')->setHeight('300px')->setTitulo('Histórico')->gerarGrafico($sql); ?>
                            </td>
                            <td>
                                <?php $grafico->setTipo(Grafico::K_TIPO_PIZZA)->setWidth('40%')->setHeight('300px')->setTitulo('Situação atual')->gerarGrafico($sql2); ?>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </div>

    <?php
    exit;
endif;

if ($_POST['carregarUsuariosSistema'])
{
    $usuariosSistema = carregarUsuariosSistema(true);

    $listing = new Listing();
    $listing->setTypePage('M');
    $listing->setActions(array('eye-open' => 'simularUsuariosSistemas'));
    $listing->setHead(array('CPF', 'Nome', 'Email'));
    $listing->listing($usuariosSistema);
    exit;
}

if ($_POST['simularUsuariosSistema'])
{
    $cpf = $_POST['cpf'];

    global $db;

    $sql = "select distinct
				u.usucpf,
				u.usunome,
				u.usuemail
				--u.susdataultacesso,
				--u.usuacesso,
				from seguranca.usuario u
				inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
				inner join seguranca.perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
				inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
				where us.suscod = 'A' and p.pflnivel >= (
                    select min(pflnivel) from seguranca.perfil
                    inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
                    and perfil.sisid = " . $_SESSION['sisid'] . " )
					and u.usucpf = '$cpf';";

    $usuario = $db->pegaLinha($sql);

    if ($usuario) {
        $_SESSION['usucpf'] = $usuario['usucpf'];
        $_SESSION['usunome'] = $usuario['usunome'];
        $_SESSION['usuemail'] = $usuario['usuemail'];
        echo 'ok';
    }

    exit;
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

$arrTheme = array('default', 'default-inverse', 'ameliaa', 'cerulean', 'cosmo', 'cyborg', 'flatly', 'journal', 'readable', 'simplex', 'slate', 'spacelab', 'united', 'yeti');
$sistemas = carregarSistemas();
$menus = menuHtml();

if ($_SESSION['sisdiretorio'] && $_SESSION["sisexercicio"] == 't')
{
    $sql = "select schemaname , relname from pg_stat_user_tables
                where schemaname = '{$_SESSION['sisdiretorio']}'
                and relname = 'programacaoexercicio'
                order by relname;";
    $tabelaExercicio = $db->pegaLinha($sql);

    if ($tabelaExercicio) {
        $sql = "select prsano as codigo, prsano as descricao,prsexerccorrente,prsexercicioaberto from " . $_SESSION['sisdiretorio'] . ".programacaoexercicio order by 1";
        $arrAnoExercicio = $db->carregar($sql);
    }
}

if (!$_SESSION['exercicioaberto'] && $_SESSION['sisexercicio'] == 't' && $tabelaExercicio) {
    $sql = sprintf("SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_SESSION['exercicio']);
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

    $sql = sprintf("SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'", $_SESSION['sisdiretorio'], $_REQUEST['exercicio']);
    $_SESSION['exercicioaberto'] = $db->pegaUm($sql);
}

// pega a página inicial do sistema
$sql = sprintf("SELECT TRIM( paginainicial ) AS paginic FROM seguranca.sistema WHERE sisid = %d", $_SESSION['sisid']);
$paginic = $db->pegaUm($sql);
?>
<!DOCTYPE html>
<html>
    <head>
        <title><?php echo NOME_SISTEMA; ?></title>
        <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <link rel='stylesheet' type='text/css' href='../includes/loading.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>

        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/blueimp/css/blueimp-gallery.min.css">
        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload.css">
        <link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui.css">
        <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
		<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" media="screen">
		
        <!-- CSS adjustments for browsers with JavaScript disabled -->
        <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-noscript.css"></noscript>
        <noscript><link rel="stylesheet" href="../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui-noscript.css"></noscript>

        <!-- jQuery JS -->
        <script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery-isloading.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

        <script language="JavaScript" src="/estrutura/js/funcoes.js"></script>

        <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
        <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
        <script language="javascript" src="/library/bootbox/bootbox.min.js"></script>
        <script type="text/javascript" language="javascript">bootbox.setDefaults({locale:'pt'});</script>

		<?php if ($theme && in_array($theme, $arrTheme)) : ?>
			<link href="../library/bootstrap-3.0.0/css/bootstrap-theme-<?= $theme ?>.css" rel="stylesheet" media="screen">
		<?php endif; ?>

        <!-- Bootstrap JS -->
        <script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>

        <!-- Prettify -->
        <script language="javascript" src="../library/prettify/prettify.js"></script>
        <script language="javascript" src="../library/prettify/run_prettify.js"></script>

        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>
        <!--<script src="js/html5shiv.js"></script>-->
 	    <!--<script src="js/respond.min.js"></script>-->
        <![endif]-->

        <!-- Chosen CSS -->
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >

        <!-- Custom CSS -->
        <link href="/library/simec/css/custom.css" rel="stylesheet" media="screen">
        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">

        <!-- Prettify -->
        <link rel="stylesheet" href="../library/prettify/prettify.css"/>
    </head>
    <body>

		<!-- begin loader -->
		<div class="loading-dialog notprint" id="loading">
	        <div id="overlay" class="loading-dialog-content">
	            <div class="ui-dialog-content">
	            	<img src="../library/simec/img/loading.gif">
					<span>
						O sistema esta processando as informações. <br/>
						Por favor aguarde um momento...
					</span>
	            </div>
	        </div>
	    </div>
		<!-- end loader -->

        <form id="form_theme" method="post" action="">
            <input type="hidden" name="theme" id="theme" value="" />
        </form>
        <script lang="javascript">
            $(document).ready(function(){
                $("#btUser").click(function(){
                        $(".mensagensAvisos").hide("slow");
                        url = window.location.href;
                        $.get(url,{limparAvisos:"true"},function(response){});
                    }
                );
                $(".confirmarAviso").click(function(e){
                    e.preventDefault();
                    url = $(this).attr('href');
                    bootbox.confirm('Deseja abrir a localização da referência agora? <br> Trabalhos não salvos serão perdidos.', function(opcao){
                        if(opcao){
                            location.href = url;
                        }
                    });
                }
                );


            });
            function changeSystem(system)
            {
                location.href = "../muda_sistema.php?sisid=" + system;
            }
            function changeTheme(theme)
            {
                $('#theme').val(theme);
                $('#form_theme').submit();
            }
        </script>
        <style>
            @media print {
                #print-head, #print-fot{
                    display:none;
                }
                .notprint {
                    display: none;

                }
                .div_rolagem{
                    display: none;
                }
                .div_rol{
                    display: none;
                }
            }

            @media screen {
                .notscreen {
                    display: none;
                }
                .div_rol{
                    display: none;
                }
            }
            .notscreen{
                border-bottom: solid 1px;
            }
            .mensagensAvisos {
                -moz-border-radius: 35%;
                -webkit-border-radius: 35%;
                border-radius: 35%;
                background: red;
                padding: 1px 4px;
                color: #fff;
                font-weight: bold;
            }
        </style>
        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen" >
            <tr bgcolor="#ffffff">
                <td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
                <td height="20" nowrap>
                    SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação<br>
                    Ministério da Educação / SE - Secretaria Executiva<br>
                    DTI - Diretoria de Tecnologia da Informação<br>
                </td>
                <td height="20" align="right" style="font-size:9px;">
                    Impresso por: <strong><?= $_SESSION['usunome']; ?></strong><br>
                    Hora da Impressão: <?= date("d/m/Y - H:i:s") ?>
                </td>
            </tr>
        </table>
        <div style="height: 60px;" class="espacador-simec" ></div>

        <div class="navbar navbar-<?php echo ( strripos($theme, '-') ) ? 'inverse' : 'default'; ?> navbar-fixed-top" style="box-shadow: #888 0px 0px 8px -3px">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <ul class="nav">
                    <li>
                        <a class="navbar-brand" href="#" onclick="javascript:changeSystem(<?php echo $_SESSION['sisid'] ?>)" style="left: -200px">
                            <!--SiMEC-->
                            <img width="100px" src="../includes/layout/planeta/img/logo.png">
                        </a>
                    </li>
                </ul>
            </div>
            <form id="setperfil" name="setperfil" action="<?= $_SESSION['sisarquivo'] ?>.php?modulo=<?= $paginic ?>" method="post">
                <input type="hidden" id="exercicio" name="exercicio" value=""  class="form-control" />
            </form>
            <div class="collapse navbar-collapse">
                <div class="row">
                    <div class="col-md-4 nav navbar-nav">
					<?php echo $menus ?>
                    </div>
                    <div class="col-md-3 nav navbar-nav">
                        <div class="navbar-brand" style="padding: 5px 0px 5px 0px; width: 100%; text-align: center !important;">
                            <select data-placeholder="Escolha um módulo do sistema..." class="chosen-select"  tabindex="2" onchange="javascipt:changeSystem(this.value);">
                                <option value="">Escolha um módulo do sistema...</option>
								<?php foreach ($sistemas as $sistema): ?>
                                    <option <?php if ($_SESSION['sisid'] == $sistema['sisid']) echo 'selected="true"' ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                <?php endforeach; ?>
                            </select>
                        </div>
                    </div>
                    <div class="col-md-4 nav navbar-right navbar-btn" style="text-align: right">
                        <div >
                            <div class="btn-group" title="Seus dados">
									<?php if ($arrAnoExercicio && is_array($arrAnoExercicio)): ?>
                                    <div class="btn-group">
                                        <button class="btn btn-sm btn-default dropdown-toggle" data-toggle="dropdown">
    										<?php echo $_SESSION['exercicio'] ?>
                                            &nbsp;
                                            <span class="caret"></span>
                                        </button>
                                        <ul class="dropdown-menu">
                                            <li role="presentation" class="dropdown-header"><i class="glyphicon glyphicon-calendar"></i> Exercício</li>
                                            <li role="presentation" class="divider"></li>
   											<?php foreach ($arrAnoExercicio as $anoExercicio): ?>
                                                <li>
                                                    <a href="#" onclick="javascript:alteraTema(<?php echo $anoExercicio['codigo'] ?>)">
                                                        <!--                                                        <i class="glyphicon glyphicon-calendar"></i>-->
        											<?php echo $anoExercicio['descricao'] ?>
                                                    </a>
                                                </li>
                                            <?php endforeach ?>
                                        </ul>
                                        <script>
                                            function alteraTema(id) {
                                                $('#exercicio').val(id);
                                                document.getElementById('setperfil').submit();
                                            }
                                        </script>
                                    </div>
								<?php endif ?>
                                <div class="btn-group">
                                    <button id="btUser" data-toggle="modal" data-target="#modal-user" class="btn btn-sm btn-default">
                                        <i class="glyphicon glyphicon-user"></i> &nbsp;
                                            <?php
                                            if (!empty($_SESSION['usunome'])) {
                                                $nomeUsuario = explode(' ', $_SESSION['usunome']);
                                                $nomeUsuario = reset($nomeUsuario) . ' ' . end($nomeUsuario);
                                                echo $nomeUsuario;
                                            }
                                            /*
                                             * Monta o circulo a quantidade de avisos que o usuário possui
                                             */
                                            $sql = "SELECT
                                             COUNT( 0 )
                                            FROM
                                             public.avisousuario
                                            WHERE
                                             usucpf = '{$_SESSION['usucpf']}'
                                            AND
                                             avbstatus = 'A'";
                                             $QtdAvisos = $db->pegaUm($sql);
                                             if($QtdAvisos > 0){
                                                 echo "&nbsp; <spam class=\"mensagensAvisos\">$QtdAvisos</spam>";
                                             }
                                             /* Marcar os avisos com lidos, ao pressionar o botão de ler*/
                                             if($_REQUEST['limparAvisos']){
                                                 $sql = "UPDATE public.avisousuario SET avbstatus = 'L' WHERE usucpf = '{$_SESSION['usucpf']}'";
                                                 $db->executar($sql);
                                             }
                                            ?>
                                    </button>
                                </div>
                                <!-- Grafico -->
								<?php if ($_SESSION['superuser'] == 1 || $exibirGraficoWorflow == true): ?>
 									<?php
								   		$sql = "select tpdid from workflow.tipodocumento where sisid = '{$_SESSION['sisid']}'";
								    	$tipoDocumento = $db->pegaUm($sql);
									?>
									<?php if ($tipoDocumento): ?>
										<div class="btn-group" title="Painel do workflow">
								    		<button data-toggle="modal" class="btn btn-sm btn-default" id="btPainelWorkflow">
								        		<i class="glyphicon glyphicon-stats"></i>
								        	</button>
								    	</div>
									<?php endif ?>
								<?php endif; ?>

                                <!-- Workflow -->
                                <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                                    <div class="btn-group" title="Simular usuário">
                                    	<?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                            <button data-toggle="modal" class="btn btn-sm btn-default" id="btSimularUsuario">
                                                <i class="glyphicon glyphicon-eye-open"></i>
                                            </button>
    									<?php else: ?>
                                            <button class="btn btn-sm btn-default" id="voltar_usuario_origem">
                                                <i  class="glyphicon glyphicon-eye-close"></i>
                                            </button>
    									<?php endif; ?>
                                    </div>
                                    <div class="btn-group" title="Gerenciar seus dados">
                                        <button onclick="javascript:window.location.href = '/seguranca/seguranca.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf=<?php echo $_SESSION['usucpf'] ?>'" class="btn btn-sm btn-default">
                                            <i class="glyphicon glyphicon-wrench"></i>
                                        </button>
                                    </div>
								<?php endif ?>
                                <div class="btn-group" title="Temas">
                                    <button class="btn btn-sm btn-default dropdown-toggle" data-toggle="dropdown">
                                        <i class="glyphicon glyphicon glyphicon-picture"></i>&nbsp;
                                        <span class="caret"></span>
                                    </button>
                                    <ul class="dropdown-menu">
                                        <li role="presentation" class="dropdown-header"><i class="glyphicon glyphicon glyphicon-picture"></i> Temas</li>
                                        <li role="presentation" class="divider"></li>
                                        <li>
										<?php foreach ($arrTheme as $theme): ?>
                                            <a href="#" onclick="javascript:changeTheme('<?php echo $theme ?>');">
                                            <?php echo ucfirst($theme) ?>
                                            </a>
                                        <?php endforeach ?>
                                        </li>
                                    </ul>
                                </div>
                            </div>
                            <div class="btn-group">
                                <a href="/logout.php">
                                    <button class="btn btn-sm btn-default">
                                        <i style="color: red;" class="glyphicon glyphicon-off"></i>
                                    </button>
                                </a>
                            </div>
                        </div>
                    </div>
                </div>
            </div><!--/.nav-collapse -->
            <!--</div>-->
        </div>
        <div id="top-shadow" style="display: none; ;height: 51px; background-color: red; margin-bottom: 30px; box-shadow: #000 0px -1px 18px; position: fixed; top: 0; width: 100%; z-index: 1;">
        </div>
        <script>
            function simularUsuariosSistemas(cpf) {
                console.info(cpf);
                //                        var html = '';
                $.post(window.location.href, {simularUsuariosSistema: 'true', cpf: cpf}, function(html) {
                    //                            html = html;
                    if (html == 'ok') {
                        location.reload();
                        //                                window.location.href = window.location.href;
                    }
                });
            }
            $('#voltar_usuario_origem').click(function() {

                simularUsuariosSistemas('<?php echo $_SESSION['usucpforigem']?>');
                

            });
            $('.navbar-brand').animate({left: 0}, 1500);

            $(window).scroll(function() {
                var coordenada = $(document).scrollTop();

                if (coordenada > 0) {
                    //        $('.navbar .navbar-default .navbar-fixed-top')
                    //        $('#top-shadow').animate({height: 51}, 'fast');
                    $('#top-shadow').fadeIn('fast');
                } else {
                    $('#top-shadow').fadeOut('fast');
                    //        $('#top-shadow').animate({height: 41}, 'fast');
                }

            });

        </script>
        <div style="position: relative; min-height: 100%;">
<?php if ($nomesRastros && is_array($nomesRastros)): ?>
<?php
    $arrRastros = explode('/', $_GET['modulo']);
    $acao = $_GET['acao'];
    $url = $_SERVER['SCRIPT_NAME'];

    //    ver($_SERVER, d);

    $rastros = array();
    $rastroLoop = '';
    foreach ($arrRastros as $rastro)
    {
        if (!$rastroLoop) {
            $rastroLoop = $rastro;
        }

        if (empty($nomesRastros[$rastro])) {
            $nome = ucfirst($rastro);
        } else {
            $nome = $nomesRastros[$rastro];
        }

        if ($rastroLoop == 'principal') {
            $rastros[$url . '?modulo=inicio&acao=' . $acao] = $nome;
        } else {
            $rastros[$url . '?modulo=' . $rastroLoop . '&acao=' . $acao] = $nome;
        }

        $rastroLoop .= '/' . $rastro;
    }
    ?>

                <div class="col-md-12">
                    <ul class="breadcrumb" style="margin-bottom: 5px;">
    <?php foreach ($rastros as $url => $rastro): ?>
        <?php if ($rastro == end($rastros)): ?>
                                <li><?php echo $rastro ?></li>
                            <?php else: ?>
                                <li><a href="<?php echo $url ?>"><?php echo $rastro ?></a></li>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    </ul>
                </div>
                    <?php endif; ?>