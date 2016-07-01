<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	    <script src="/emi/geral/js/emi.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
<?php
include (APPRAIZ.'www/painel/_funcoes_metas.php');

$metid = $_GET['metid'];

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	if($_POST['requisicaoAjax'] == "getComboPeriodoPorPerid"){
		if(!$_POST['perid'] && $_POST['dpeid']){
			$perid = $db->pegaUm("select perid from painel.detalheperiodicidade where dpeid = {$_POST['dpeid']}");
		}else{
			$perid = $_POST['perid'];
		}
		if($perid){
			getComboPeriodoPorPerid($perid,false,$_POST['dmiid'],$metid);
		}else{
			echo "Selecione o período.";
		}
	}else{
		$_POST['requisicaoAjax']();
	}
	exit();
}

if($_POST['requisicao']){
	$resultado = $_POST['requisicao']();
	
	//Cria a Demanda para o usuário
	if($metid){
		if($resultado['dmiid']){
			salvarPendenciaExecutor($resultado['dmiid']);
		}
	}
	
	echo "<script>alert('Meta salva com sucesso.');window.opener.location.href=window.opener.location;window.close();</script>";
}

$sql = "select 
			ind.indid,
			ind.indnome,
			ind.indqtdevalor,
			met.perid,
			ume.umedesc,
			ind.unmid,
			ind.estid
		from 
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid 
		where 
			metid = $metid";

$arrDados = $db->pegaLinha($sql);
if($arrDados){
	extract($arrDados);
}	

if($_GET['dmiid']){
	$sql = "select
			*
		from
			painel.detalhemetaindicador dmi
		inner join
			painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
		where
			dmi.dmiid = {$_GET['dmiid']}
		and
			dmi.dmistatus = 'A'";

	$arrValores = $db->pegaLinha($sql);
	if($arrValores){
		extract($arrValores);	
	}
	
	if($dpeid){
		$_POST['dpeid'] = $dpeid;
	}
}

montar_titulo_projeto( "Valor da Meta" );
?>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link href="../includes/jquery-ui-1.8.18.custom/development-bundle/themes/base/jquery.ui.all.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.link{cursor: pointer;}
	.hidden{display:none}
</style>
<?php $formatoinput = pegarFormatoInput($arrDados['indid']);
	  $formatoinput['campovalor']['mascara'] = !$formatoinput['campovalor']['mascara'] ? "###.###.###.###.###,##" : $formatoinput['campovalor']['mascara']; ?>
<script>

function alteraPeriodo(perid)
{
	executarAjax("requisicaoAjax=getComboPeriodoPorPerid&perid="+perid+"&medid=<?php echo $metid ?>",$("#combo_dpeid"));
}

$(function() {
	if($( "#entid_validador",window.opener.document ).val()){
		$( "#tr_data_validacao" ).show();
	}else{
		$( "#tr_data_validacao" ).hide();
	}
});

</script>
<form name="formulario_valor_metas" id="formulario_valor_metas"  method="post" action="" >
	<input type="hidden" id="metid" name="metid" value="<?php echo $_REQUEST['metid'] ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr style="display:none" >
			<td width="25%" class="SubTituloDireita" >Tipo de Período:</td>
			<td id="combo_perid">
				<?php $sql = "	select
									perid as codigo,
									perdsc as descricao
								from
									painel.periodicidade
								where
									pernivel >= (select pernivel from painel.periodicidade where perid = $perid)"; 
					$db->monta_combo('perid',$sql,"S",'Selecione o Período','alteraPeriodo','','','','S','','',$perid);
				?>
			</td>
		</tr>
		<?php if($unmid != UNIDADEMEDICAO_BOLEANA): ?>
			<?php if($estid == 2): ?>
				<tr id="tr_faixas" style="display:<?php echo $display_faixas ?>" >
					<td class="SubtituloDireita" >Faixas Avaliativas:</td>
					<td>
						<style>
						#demo-frame > div.demo { padding: 10px !important; };
						</style>
						<script>
						$(function() {
							$( "#slider-range" ).slider({
								range: true,
								min: 0,
								max: 100,
								values: [ <?php echo $dmdestavel ? $dmdestavel : 33 ?>, <?php echo $dmdcritico ? $dmdcritico : 66 ?> ],
								slide: function( event, ui ) {
									$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
									$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
									$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
									$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
									$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 0 ));
									$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 1 ));
								}
							});
							$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
							$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
							$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
							$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
							$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 0 ));
							$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 1 ));
						});
						</script>
						<div class="demo">
						<div id="slider-range" style="width:300px"></div>
						</div>
						<div style="margin-top:5px;">
						<table width="300px" border="0" cellpadding="1" >
							<tr>
								<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#228B22"></div></td>
								<td><b>Estável:</b> até <span id="valor_estavel" ></span>.</td>
							</tr>
							<tr>
								<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FFFF00"></div></td>
								<td><b>Atenção:</b> entre <span id="valor_ini_estavel" ></span> e <span id="valor_fim_estavel" ></span>.</td>
							</tr>
							<tr>
								<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FF0000"></div></td>
								<td><b>Crítico:</b> maior que <span id="valor_critico" ></span>.</td>
							</tr>
						</table>
						<input type="hidden" name="dmdcritico" id="hdn_critico" />
						<input type="hidden" name="dmdestavel" id="hdn_estavel" />
						</div>
					</td>
				</tr>
			<?php else: ?>
				<tr id="tr_faixas" style="display:<?php echo $display_faixas ?>" >
					<td class="SubtituloDireita" >Faixas Avaliativas:</td>
					<td>
						<style>
						#demo-frame > div.demo { padding: 10px !important; };
						</style>
						<script>
						$(function() {
							$( "#slider-range" ).slider({
								range: true,
								min: 0,
								max: 100,
								values: [ <?php echo $dmdcritico ? $dmdcritico : 33 ?>, <?php echo $dmdestavel? $dmdestavel : 66 ?> ],
								slide: function( event, ui ) {
									$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
									$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
									$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
									$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
									$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 1 ));
									$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 0 ));
								}
							});
							$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
							$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
							$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
							$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
							$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 1 ));
							$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 0 ));
						});
						</script>
						<div class="demo">
						<div id="slider-range" style="width:300px"></div>
						</div>
						<div style="margin-top:5px;">
						<table width="300px" border="0" cellpadding="1" >
							<tr>
								<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FF0000"></div></td>
								<td><b>Crítico:</b> até <span id="valor_critico" ></span>.</td>
							</tr>
							<tr>
								<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FFFF00"></div></td>
								<td><b>Atenção:</b> entre <span id="valor_ini_estavel" ></span> e <span id="valor_fim_estavel" ></span>.</td>
							</tr>
							<tr>
								<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#228B22"></div></td>
								<td><b>Estável:</b> maior que <span id="valor_estavel" ></span>.</td>
							</tr>
						</table>
						<input type="hidden" name="dmdcritico" id="hdn_critico" />
						<input type="hidden" name="dmdestavel" id="hdn_estavel" />
						</div>
					</td>
				</tr>
			<?php endif; ?>
		<?php endif; ?>
		<tr>
			<td width="25%" class="SubTituloDireita" >Data de Execução:</td>
			<td >
				<?php $dmidataexecucao = $dmidataexecucao ? formata_data($dmidataexecucao) : ""; echo campo_data2("dmidataexecucao","S","S","","","","",$dmidataexecucao); ?>
			</td>
		</tr>
		<tr id="tr_data_validacao">
			<td class="SubTituloDireita" >Data de Validação:</td>
			<td >
				<?php $dmidatavalidacao = $dmidatavalidacao ? formata_data($dmidatavalidacao) : ""; echo campo_data2("dmidatavalidacao","S","S","","","","",$dmidatavalidacao); ?>
			</td>
		</tr>
		<?php if($perid != 5): ?>
			<tr>
				<td class="SubTituloDireita" >Data da Meta:</td>
				<td >
					<?php $dmidatameta = $dmidatameta ? formata_data($dmidatameta) : ""; echo campo_data2("dmidatameta","S","S","","","","",$dmidatameta); ?>
				</td>
			</tr>
			<tr>
				<td width="25%" class="SubTituloDireita" >Período da Meta:</td>
				<td id="combo_dpeid"><?php getComboPeriodoPorPerid($perid,false,$dmiid,$metid) ?></td>
			</tr>
		<?php endif; ?>
		<tr style="display:<?php echo $unmid == UNIDADEMEDICAO_BOLEANA ? "none" : "" ?>" >
			<td width="25%" class="SubTituloDireita" ><?php echo $arrDados['umedesc']; ?>:</td>
			<td><?$dmiqtde = $unmid == UNIDADEMEDICAO_BOLEANA ? "1" : $dmiqtde;$dmiqtde = str_replace(".00","",$dmiqtde); echo campo_texto('dmiqtde','S',"S",'',20,20,$formatoinput['mascara'],'');?></td>
		</tr>
		<?php if($arrDados['indqtdevalor'] == "t"): ?>
			<tr style="display:<?php echo $unmid == UNIDADEMEDICAO_BOLEANA ? "none" : "" ?>" >
				<td width="25%" class="SubTituloDireita" >Valor:</td>
				<td><? $dmivalor = str_replace(".00","",$dmivalor); echo campo_texto('dmivalor','N',"S",'',20,20,$formatoinput['campovalor']['mascara'],'');?></td>
			</tr>
		<?php endif; ?>
		<tr>
			<td width="25%" class="SubtituloTabela" ></td>
			<td class="SubtituloTabela" >
				<input type="hidden" id="dmiid" name="dmiid" value="<?php echo $dmiid ?>" />
				<input type="hidden" name="requisicao" value="salvarValorMeta" />
				<input type="button" id="btn_salvar" value="Salvar" onclick="salvarValorMeta()" />
				<input type="button" id="btn_cancelar" value="Cancelar" onclick="window.close()" />
			</td>
		</tr>
	</table>
</form>