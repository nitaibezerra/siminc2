<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */

header("Content-Type: text/html; charset=ISO-8859-1",true);

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/workflow.php';
include_once '_funcoes.php';
include_once '_constantes.php';


// abre conexão com o servidor de banco de dados
$db = new cls_banco();


if(!$_SESSION["usucpf"]){
	echo '<script>
    		alert("É necessário estar logado no SIMEC!");
    		location.href="http://simec.mec.gov.br";
    	 </script>';
    exit;
}

// Realiza a alteração do estado EM ATENDIMENTO para FINALIZADA.
if($_REQUEST['finalizar']){
	
	$_SESSION['dmdid'] = $_REQUEST['dmdid'];
	
	//pega DOCID
    $sql = "SELECT docid FROM demandas.demanda WHERE dmdid = ".$_REQUEST['dmdid'];
    $docid = $db->PegaUm( $sql );
    
    $dados = array();
    $comentario = "Demanda Finalizada pelo CPF: ".$_SESSION['usucpf']." NOME: ".$_SESSION['usunome'];
    
    if($docid){
      $ok = wf_alterarEstado( $docid, 191, $comentario, $dados );
    }
	
    unset($_SESSION['dmdid']);
    
    if($ok){
        echo '<script>
	    		alert("Demanda Finalizada com Sucesso.");
	    		location.href="popPainelGerenciaGabinete.php";
	    	 </script>';
    	exit;
    }else{
    	echo '<script>
	    		alert("Erro, a demanda não foi finalizada.");
	    		location.href="popPainelGerenciaGabinete.php";
	    	 </script>';
    	exit;
    }

}

// Realiza a alteração do estado EM ATENDIMENTO para CANCELADA.
if($_REQUEST['cancelar']){

	$_SESSION['dmdid'] = $_REQUEST['dmdid'];
	
	//pega DOCID
    $sql = "SELECT docid FROM demandas.demanda WHERE dmdid = ".$_REQUEST['dmdid'];
    $docid = $db->PegaUm( $sql );
    
    $dados = array();
    $comentario = "Demanda Cancelada pelo CPF: ".$_SESSION['usucpf']." NOME: ".$_SESSION['usunome'];

    if($docid){
       $ok = wf_alterarEstado( $docid, 374, $comentario, $dados );
    }

    unset($_SESSION['dmdid']);
    
	if($ok){
	    echo '<script>
	    		alert("Demanda Cancelada com Sucesso.");
	    		location.href="popPainelGerenciaGabinete.php";
	    	 </script>';
    	exit;
    }else{
    	echo '<script>
	    		alert("Erro, a demanda não foi cancelada.");
	    		location.href="popPainelGerenciaGabinete.php";
	    	 </script>';
    	exit;
    }
    
}



?>
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
<!--        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" /> -->
        <meta content="IE=9" http-equiv="X-UA-Compatible" />

        <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

        <!-- CSS Inicio -->
        
	        <!-- Bootstrap -->
			<link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
			
			<!-- jQuery UI -->
	        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
	    	<link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>
	    	
			<!-- Custom -->
	        <link href="/estrutura/temas/default/css/css_reset.css" rel="stylesheet">
	        <link href="/estrutura/temas/default/css/estilo.css" rel="stylesheet">        
		<!-- CSS Fim -->
		
        <!-- JS Inicio -->
	        <!-- jQuery -->
			<script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
	    	<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
	    	<script src="../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
	    	
	    	<!-- Bootstrap -->
	    	<script src="../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
	        
	        <!-- jQuery UI -->
	    	<script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
    	<!-- JS Fim -->
        

        <style type="text/css">

			.modal-dialog-large{
				width: 1000px !important; 
			}

            .row {margin: 2px;}
            .box {padding: 5px; margin-bottom: 0px;}
            .box-principal {padding: 3px;}
            .panel { margin: 2px;
                border: 0px red;
                box-shadow: 0px 0px 4px 0px black;
            }

            .panel *{
                color: #fff;
            }

            .panel-heading{
                padding: 10px 2px;
            }

            .panel-body{
                color: #000;
                padding-top: 3px;
            }

			.panel-azul{ background: #0020C2; }
            .panel-atrasado{ background: #f00; }
            .panel-em-dia{ background: orange; }
            .panel-a-vencer{ background: green; }
            .panel-zerado{ background: #000; color: white; }

			.panel-body-azul{ background: #1E90FF;  }
            .panel-body-atrasado{ background: #FFEDED; }
            .panel-body-em-dia{ background: #fcf8e3; }
            .panel-body-a-vencer{ background: #dff0d8; }
            .panel-body-zerado{ background: #eee; }

            .panel h3{
                font-size: 11px !important;
                font-weight: bold;
                text-align: center;
                text-shadow: 0px 0px 4px #000;
            }
            
            .label-danger {background: red;}
            .label-warning {background: #FFA500; }
            .label-success {background: green;}
			.label-tabela {
            	font-size: 14px !important;
			    border-spacing: 6px;
			    font-weight: bold;
            }
            
			/*
			.label-tabela {
            	font-size: 14px !important;
            	border-collapse: separate;
			    border-spacing: 6px;
            	
            }
            */
            
            .box-green  {background: #0F6D39; padding: 20px;}

            .box-orange {background: #EE9200; padding: 20px;}

            tr.danger th,  tr.danger td  {background: #f2dede !important;}
            tr.warning th, tr.warning td {background: #fcf8e3 !important;}
            tr.success th, tr.success td {background: #dff0d8 !important;}

            .highcharts-container{
                margin: 0 !important;
            }
            
            .tituloGrafico{
            	text-align: center;
            	color: #fff;
            }

            .item-menu{
                margin-left: 5px;
            }

            .link-menu{
                width: 105%;
            }
        </style>

        <script type="text/javascript">
        	$(function(){
        	
//        		$('.data').datepicker();

        		$('.ver-detalhes').click(function(){
            		$('#div-entregas').load('popupDemandasPainelGabinete.php?usucpf='+$(this).attr('usucpf'));
        			$('#myModal').modal();
        			
        			
        		});

        		$('.link-cadastrar').click(function(){
            		$('#div-cadastrar').load('popupCadDemandasGabinete.php?usucpf='+$(this).attr('usucpf'), null , function(){
            			$('.data').datepicker();
            		}  );
        			$('#myModalCadastrar').modal();
        			
        			
        		});
        		
        	});

   		</script>

    </head>

    <body>
        <!-- // Barra do Governo -->
        <?php //include_once "../barragoverno.php"; ?>


        <nav class="navbar navbar-inverse" role="navigation">
            <div class="container-fluid">
                <div class="navbar-header pull-left">
                    <a href="/demandas/demandas.php?modulo=principal/lista&acao=A"><img src="/estrutura/temas/default/img/logo-simec.png"></a>
                </div>
                <div>
                    <ul class="nav navbar-nav">
                        <li class="pull-left"><a href="#" class="link-cadastrar link-menu"><span class="glyphicon glyphicon-plus-sign"></span> <span class="hidden-xs pull-right item-menu">Cadastrar Demanda</span></a></li>
<!--                        <li class="pull-left"><a href="#" class="link-menu"><span class="glyphicon glyphicon-search"></span> <span class="hidden-xs pull-right item-menu">Filtrar</span></a></li>-->
                    </ul>
                </div>
            </div>
        </nav>
        <?php

        $_REQUEST['ordid']  = $_REQUEST['ordid']  ? $_REQUEST['ordid']  : 23;
        $_REQUEST['celid']  = $_REQUEST['celid']  ? $_REQUEST['celid']  : 49;
        $_REQUEST['funcao'] = $_REQUEST['funcao'] ? $_REQUEST['funcao'] : 1191;
        
		if($_REQUEST["funcao"]) $andTec = " and ur.pflcod = ".$_REQUEST["funcao"];
        if($_REQUEST["celid"]){
			$andCel = " and ur.celid = ".$_REQUEST["celid"];
		}else{
			if($andCel){
				$andCel = $andCel;
			}
			else{
				$andCel = " and ur.celid = 2";	
			}
						
		}
		
		if($_REQUEST["ordid"] != '1' && $_REQUEST["ordid"] != '2' && $_REQUEST["ordid"] != '23'){
			$andCel = '';
			$andTec = '';
			$andOrd = " and ur.ordid = ".$_REQUEST["ordid"];
		}
		if($_REQUEST["ordid"] == '2' || $_REQUEST["ordid"] == '23'){
			$andTec = '';
		}
		
		
		$perfilUser	= arrayPerfil();
		if ( !in_array(DEMANDA_PERFIL_SUPERUSUARIO, $perfilUser) ){
			if ( in_array(DEMANDA_PERFIL_EQUIPE, $perfilUser) ) {
				$andEquipe = " and u.usucpf = '".$_SESSION["usucpf"]."'";
			}
		} 

        $sql = "SELECT DISTINCT
                    u.usucpf,
                    u.usunome
                FROM
                    seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                WHERE
                    ur.rpustatus = 'A' AND
                    us.susstatus = 'A' AND
                    us.suscod = 'A'
                    $andTec
					$andCel
					$andOrd
					$andEquipe
                ORDER BY u.usunome";
		 //dbg($sql,1);
         $usuarios = $db->carregar( $sql );
        ?>
        
        <div class="row">
        	
            <?php /*
        	<div align="center" style="cursor:pointer; vertical-align: bottom;  margin-top: 1px; color: #fff; font-weight: bold;" class="titulo_box" >
        		<?
        		if($_REQUEST["ordid"] == '1' || $_REQUEST["ordid"] == '23'){
        			echo strtoupper( ($_REQUEST['celid'] ? $db->pegaUm("SELECT celnome FROM demandas.celula WHERE celid = ".$_REQUEST['celid']) : '') );
        		}
        		else{
        			echo strtoupper( $db->pegaUm("SELECT orddescricao FROM demandas.origemdemanda WHERE ordid = ".$_REQUEST['ordid']) );
	        		if($_REQUEST["ordid"] == '2' || $_REQUEST["ordid"] == '23'){
	        			echo ' - '.strtoupper( ($_REQUEST['celid'] ? $db->pegaUm("SELECT celnome FROM demandas.celula WHERE celid = ".$_REQUEST['celid']) : '') );
	        		}
        		}
        		?>
        	</div>
            */ ?>

            <div class="col-md-12 box-principal">
                <div class="col-md-12">

                    <?php
					if($usuarios){
						
                    	foreach ($usuarios as $usuario) {

                    	//pega o analista com a demanda mais atrasada
                        $sql = "SELECT
                                    u.usunome as analista
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                LEFT JOIN
                                	seguranca.usuario u ON u.usucpf = d.usucpfanalise 
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND d.dmddatafimprevatendimento < CURRENT_DATE
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                    and d.celid = 49
                                order by d.dmddatafimprevatendimento
                                limit 1";
                        $analista = $db->PegaUm( $sql );
                        
                        //total demandas atrasadas
                        $sql = "SELECT
                                    count(*) as qtd
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND d.dmddatafimprevatendimento < CURRENT_DATE
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                    and d.celid = 49
                                ";
                        $atrasados = $db->PegaUm( $sql );

                        //total demandas que vencem hoje
                        $sql = "SELECT
                                    count(*) as qtd
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                    and d.celid = 49
                                ";
                        $emDia = $db->PegaUm( $sql );

                        //total demandas em dia
                        $sql = "SELECT
                                    count(*) as qtd
                                FROM
                                    demandas.demanda as d
                                LEFT JOIN
                                    workflow.documento doc ON doc.docid       = d.docid
                                LEFT JOIN
                                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                                WHERE
                                    d.usucpfexecutor = '".$usuario['usucpf']."'
                                    AND d.usucpfdemandante is not null
                                    AND d.dmdstatus = 'A'
                                    AND ed.esdstatus = 'A'
                                    AND doc.esdid in (91,92,107,108)
                                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                    and d.celid = 49
                                ";
                        $aVencer = $db->PegaUm( $sql );
                        

                        $class = 'zerado';

                        if ($atrasados) {
                            $class = 'atrasado';
                        } elseif($emDia) {
                            $class = 'em-dia';
                        } elseif($aVencer) {
                            $class = 'a-vencer';
                        }

                        ?>

                        <div class="col-md-2 col-sm-4 col-xs-6 box">
                            <div class="panel">
                                <div class="panel-heading panel-<?php echo $class; ?> ver-detalhes" usucpf="<?php echo $usuario['usucpf']; ?>">
                                    <h3 class="panel-title">
                                        <?php
                                        echo substr($usuario['usunome'], 0, strpos($usuario['usunome'], ' ')) . substr($usuario['usunome'], strrpos($usuario['usunome'], ' '));
                                        ?>
                                    </h3>
                                </div>
                                <div class="panel-body panel-body-<?php echo $class; ?> ver-detalhes" usucpf="<?php echo $usuario['usucpf']; ?>" >
                                    <div style="width:90px; margin: 0 auto;">
                                        <span class="label label-danger"> <?php echo (int) $atrasados; ?></span>
                                        <span class="label label-warning"><?php echo (int) $emDia; ?></span>
                                        <span class="label label-success"><?php echo (int) $aVencer; ?></span>
                                    </div>
                                </div>
                            </div>
                        </div>

                    <?php }
					}
					else{
						echo '<center><font color="white"><b>Não existem registros.</b></font></center>';
					} ?>
                    <div class="clearfix"></div>
                </div>

                
            </div>
        </div>



        <div class="modal fade" id="myModal">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                        <h4 class="modal-title">Entregas</h4>
                    </div>
                    <div class="modal-body" id="div-entregas"></div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    </div>
                </div><!-- /.modal-content -->
            </div><!-- /.modal-dialog -->
        </div><!-- /.modal -->

        <div class="modal fade" id="myModalCadastrar">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                        <h4 class="modal-title">Cadastrar Demanda</h4>
                    </div>
                    <div class="modal-body" id="div-cadastrar"></div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    </div>
                </div><!-- /.modal-content -->
            </div><!-- /.modal-dialog -->
        </div><!-- /.modal -->

       
    </body>
</html>

