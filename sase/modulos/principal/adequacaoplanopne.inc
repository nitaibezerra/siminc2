<?php
ob_start();

global $db;

// --------------- Depend???ncias
include_once '../../sase/classes/AvaliadorEducacional.class.inc';
include_once '../../sase/classes/ExperienciaAvaliadorEducacional.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//include APPRAIZ . 'includes/funcoes.inc';
//include APPRAIZ . 'www/sase/_funcoes.php';
// --

$avaliadoreducacional = new AvaliadorEducacional();

include APPRAIZ . 'includes/cabecalho.inc';

$titulo = 'Adequa??????o dos Planos de Educa??????o ao PNE';


?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<div id='cabecalhoSistema'>
	<center><h3><?=$titulo?></h3></center>
</div>

<!-- html -->
<div id="container">
	<div class="row">
		<div class="row col-md-12">
			<form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
				<input type="hidden" id="requisicao" name="requisicao" value="" />
				<input name="formulario" value="1" type="hidden"/>
				<input name="acao" value="pesquisa" type="hidden"/>
				<input name="aveid" value="<?=@$_GET['aveid']?>" type="hidden"/>
				<input name="usucpf" value="<?=@$_SESSION['usucpf']?>" type="hidden"/>
								
				<div class="row col-md-12">
					<div class="well">
						<fieldset>
							
							<div class="form-group">
							    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">Perfil:</label>
							
							    <div class="col-lg-8 col-md-8 ">
						            <?php echo $avaliadoreducacional->getTipos() ?>
							    </div>
							</div>
							<div class="form-group">
							    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">UF:</label>
							
							    <div class="col-lg-8 col-md-8 ">
						            <?php echo $avaliadoreducacional->getUfs('estuf') ?>
							    </div>
							</div>
							<div class="form-group">
							    <label for="dmdtipo" class="col-lg-4 col-md-4 control-label obrigatorio">Avaliador Educacional:</label>
							
							    <div class="col-lg-8 col-md-8 ">
						            <?php echo $avaliadoreducacional->gerComboAvaliadores(); ?>
							    </div>
							</div>
							
							<div class="form-group">
							    <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Per???odo:</label>
							
							    <div class="col-lg-8 col-md-8 ">
							        <input id="avedtini" name="avedtini" type="text" class="campoData form-control"
							               placeholder="">
							        &nbsp&nbsp ??? &nbsp&nbsp
							        <input id="avedtfim" name="avedtfim" type="text" class="campoData form-control"
							               placeholder="">
				               </div>
							</div>
																				
						</fieldset>
					</div>
				</div>
			</form>
		</div>
	</div>
</div>
