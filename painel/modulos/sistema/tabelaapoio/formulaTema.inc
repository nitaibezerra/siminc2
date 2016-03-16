<?php
include APPRAIZ . 'www/painel/_funcoes_formula_tema.php';

ini_set( "memory_limit", "2048M" );
set_time_limit(0);

if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();	
}

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
monta_titulo("Fórmulas","Criação e edição de fórmulas para os temas dos mapas.");

function salvarTemaMapa()
{
	global $db;
	extract($_POST);
	if($tmaid){
		$sql = "update 
					mapa.tema
				set
					tmadsc = '$tmadsc',
					tmacor = '$tmacor',
					tpdidformula = '$tpdidformula',
					tmaformula = '$tmaformula'
				where
					tmaid = $tmaid";
		$db->executar($sql);
	}else{
		$sql = "insert into
				mapa.tema
					(tmadsc,tpdid,tmaagreg,tmacor,tmaformula,tpdidformula)
				values
					('$tmadsc',".TIPO_MAPA_FORMULA.",false,'$tmacor','$tmaformula','$tpdidformula')
				returning 
					tmaid";
		$tmaid = $db->pegaUm($sql);
	}
	$db->commit();
	$db->sucesso("sistema/tabelaapoio/formulaTema","");
	exit;
}

function excluirFormulaMapa()
{
	global $db;
	extract($_POST);
	$sql = "update 
				mapa.tema
			set
				tmastatus = 'I'
			where
				tmaid = $tmaid";
	$db->executar($sql);
	$db->commit();
	$db->sucesso("sistema/tabelaapoio/formulaTema","");
}

function listaTemaMapaPopup()
{
	global $db;
	?>
	<html>
		<head>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
	<body>
		<script>
			function addTema(tmaid,tmadsc)
			{
				var hdn_tmaid = $("#hdn_tmaid",window.opener.document).val();
				if(!hdn_tmaid){
					$("#hdn_tmaid",window.opener.document).val(tmaid);
				}else{
					$("#hdn_tmaid",window.opener.document).val(hdn_tmaid + "," + tmaid);
				}
				
				var formula = $("#tmaformula",window.opener.document).val();
				$("#tmaformula",window.opener.document).val(formula + "{Tema: " + tmaid + "}");
				
				if(hdn_tmaid.search(tmaid) < 0){
					var detalhe = $("#div_detalhe_formula",window.opener.document).html();
					$("#div_detalhe_formula",window.opener.document).html(detalhe + "<p><b>Tema: " + tmaid + "</b> - "+ tmadsc + "</p>");
				}
				
			}
		</script>
		<?php monta_titulo("Temas","Selecione os temas para compor a fórmula"); ?>
		<form name="formulario" id="formulario"  method="post" action="" >
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td width="25%" class="SubtituloDireita" >Tema:</td>
				<td><?php $tmadsc = $_POST['tmadsc']; echo campo_texto("tmadsc","S","S","",40,250,"","") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita" ></td>
				<td class="SubtituloEsquerda" ><input type="submit" name="btn_buscar" value="Buscar" /><input type="button" name="btn_todos" onclick="window.location.href=window.location" value="ver Todos" /></td>
			</tr>
		</table>
	</form>
	<?php
	if($_POST['tmadsc']){
		$arrWhere[] = "removeacento(tmadsc) ilike(removeacento('%{$_POST['tmadsc']}%'))";
		$arrWhere[] = "tmaid::text ilike('%{$_POST['tmadsc']}%')";
	}
	$sql = "select
			'<img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer\" onclick=\"addTema(' || tmaid || ',\'' || tmadsc || '\')\" />' as acao,
			tmaid || ' - ' || tmadsc as tema
		from
			mapa.tema
		where
			tmastatus = 'A'
		".($arrWhere ? " and (".implode(" or ",$arrWhere).")" : "")."
		order by
			tmadsc";
	$arrCab = array("Ação","Tema");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
	?>
		</body>
	</html>
	<?php
	exit;
}


function testarFormulaTema()
{
	global $db;
	$tmaformula = $_POST['tmaformula'];
	$arrResultadoFinal = trataFormula($tmaformula);
	if($arrResultadoFinal){
		$n=0;
		echo '<table border="0" cellspacing="0" cellpadding="3" align="left" width="400"  style="margin-top:5px;border-top: none; border-bottom: none;">';
		echo "<tr bgcolor='#D5D5D5' ><td><b>Município/UF</b></td><td><b>Qtde.</b></td></tr>";
		foreach($arrResultadoFinal as $muncod => $valor){
			if($n <= 10){
				$sql = "select mundescricao || '/' || estuf as mundescricao from territorios.municipio where muncod = '$muncod'";
				$mun = $db->pegaUm($sql);
				$cor = $n%2 == 1 ? "#f0f0f0" : "#FFFFFF";
				echo "<tr bgcolor='$cor' ><td >$mun</td><td style='color:blue;text-align:right' >".number_format($valor,2,",",".")."</td></tr>";
			}else{
				exit;
			}
			$n++;
		}
		echo "</table>";
	}else{
		echo "<p style=\"color:#990000\" ><b>Fórmula inválida!</b></p>";
	}
	exit;
}


function listaIndicadorPopup()
{
	global $db;
	?>
	<html>
		<head>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		</head>
	<body>
		<script>
			function addTema(indid,indnome)
			{
				var hdn_indid = $("#hdn_indid",window.opener.document).val();
				var dpe = $("#indid_dpe_" + indid).val();
				if(!hdn_indid){
					$("#hdn_indid",window.opener.document).val(indid + "_" + dpe);
				}else{
					$("#hdn_indid",window.opener.document).val(hdn_indid + "," + indid + "_" + dpe);
				}
				
				var formula = $("#tmaformula",window.opener.document).val();
				$("#tmaformula",window.opener.document).val(formula + "{Indicador: " + indid + "_" + $("#indid_dpe_" + indid + " option:selected").text() + "}");
				
				if(hdn_indid.search( indid + "_" + dpe) < 0){
					var detalhe = $("#div_detalhe_formula",window.opener.document).html();
					$("#div_detalhe_formula",window.opener.document).html(detalhe + "<p><b>Indicador: " + indid + "</b> - "+ indnome + " em " + $("#indid_dpe_" + indid + " option:selected").text() + "</p>");
				}
				
			}
		</script>
		<?php monta_titulo("Indicadores","Selecione os indicadores para compor a fórmula"); ?>
		<form name="formulario" id="formulario"  method="post" action="" >
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td width="25%" class="SubtituloDireita" >Indicador:</td>
				<td><?php $indnome = $_POST['indnome']; echo campo_texto("indnome","S","S","",40,250,"","") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita" ></td>
				<td class="SubtituloEsquerda" ><input type="submit" name="btn_buscar" value="Buscar" /><input type="button" name="btn_todos" onclick="window.location.href=window.location" value="ver Todos" /></td>
			</tr>
		</table>
	</form>
	<?php
	if($_POST['indnome']){
		$arrWhere[] = "removeacento(indnome) ilike(removeacento('%{$_POST['indnome']}%'))";
		$arrWhere[] = "ind.indid::text ilike('%{$_POST['indnome']}%')";
	}
	$sql = "select
			'<img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer\" onclick=\"addTema(' || ind.indid || ',\'' || indnome || '\')\" />' as acao,
			ind.indid || ' - ' || indnome as indnome,
			ind.indid
		from
			painel.indicador ind
		where
			indstatus = 'A'
		".($arrWhere ? " and (".implode(" or ",$arrWhere).")" : "")."
		order by
			indnome";
	$arrCab = array("Ação","Indicador","Referência");
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $n => $dado){
			$sqlI = "select 
						dpe.dpeid as codigo,
						dpedsc as descricao
					from 
						painel.detalheperiodicidade dpe
					inner join
						painel.seriehistorica seh ON seh.dpeid = dpe.dpeid
					where
						sehstatus != 'I'
					and
						dpe.dpestatus = 'A'
					and
						seh.indid = {$dado['indid']}
					order by
						dpe.dpedatainicio";
			$arrOpcoes = $db->carregar($sqlI);
			if($arrOpcoes){
				unset($arrDados[$n]['indid']);
			$arrDados[$n]['referencia'] = $db->monta_combo("indid_dpe_{$dado['indid']}",$arrOpcoes,"S","","","","","150","N","indid_dpe_{$dado['indid']}",true);
			}else{
				unset($arrDados[$n]);
			}
		}
	}
	$db->monta_lista_array($arrDados,$arrCab,50,10,"N","center");
	?>
		</body>
	</html>
	<?php
	exit;
}

if($_POST['tmaid']){
	$sql = "select * from mapa.tema where tmaid = {$_POST['tmaid']}";
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		extract($arrDados);
	}
}
?>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>
<link href="../includes/JQuery/jquery-color-picker/colorPicker.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-color-picker/jquery.colorPicker.min.js"></script>
<script type="text/javascript">
	$(function() {    
		$("#tmacor").colorPicker();
	});

	function salvarTemaMapa()
	{
		$("#formulario").submit();	
	}
	function editarFormulaMapa(tmaid)
	{
		$("[name='tmaid']").val(tmaid);
		$("#requisicao").val("");
		$("#formulario").submit();
	}
	function excluirFormulaMapa(tmaid)
	{
		if(confirm("Deseja realmente excluir este tema?")){
			$("[name='tmaid']").val(tmaid);
			$("#requisicao").val("excluirFormulaMapa");
			$("#formulario").submit();
		}
	}
	
	function addFormulaTema()
	{
		janela("painel.php?modulo=sistema/tabelaapoio/formulaTema&acao=A&requisicao=listaTemaMapaPopup",800,600,"Temas");
	}
	
	function addFormulaIndicador()
	{
		janela("painel.php?modulo=sistema/tabelaapoio/formulaTema&acao=A&requisicao=listaIndicadorPopup",800,600,"Indicadores");
	}
	
	function testarFormulaTema()
	{
		
		$("#requisicao").val("testarFormulaTema");
		var parametros = $("#formulario").serialize();
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicao=testarFormulaTema&" + parametros,
		   success: function(msg){
		   		$('#div_testa_formula').html( msg );
		   		$("#requisicao").val("salvarTemaMapa");
		   }
		 });
	}
</script>
<form name="formulario" id="formulario"  method="post" action="" target="" >
	<input type="hidden" id="requisicao" name="requisicao" value="salvarTemaMapa" />
	<input type="hidden" name="tmaid" value="<?php echo $tmaid ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita" >Tema:</td>
			<td><?php echo campo_texto("tmadsc","S","S","",60,250,"","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Cor:</td>
			<td>
				<div class="controlset"><input id="tmacor" type="text" name="tmacor" value="<?php echo $tmacor ? $tmacor : "#FFFFFF" ?>" /></div>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Tipo de Dado:</td>
			<td>
				<?php $sqlTipo = "select tpdid as codigo, tpddsc as descricao from mapa.tipodado where tpdstatus = 'A' and tpdid not in (5,6) order by tpddsc"; ?>
				<?php $db->monta_combo('tpdidformula',$sqlTipo,'S','Selecione...','','') ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Fórmula:</td>
			<td>
				<?php echo campo_textarea("tmaformula","S","S","",81,5,255) ?>
				<input type="button" id="btn_formula_tema" onclick="addFormulaTema()" value="Inserir Tema" />
				<input type="button" id="btn_formula_indicador" onclick="addFormulaIndicador()" value="Inserir Indicador" />
				<input type="button" id="btn_formula_testar" onclick="testarFormulaTema()" value="Testar Fórmula" />
				<?php 
				if($tmaformula){
					$hdn_tmaid = explode("{Tema:",str_replace(array("{Indicador:"," ","-","+","/","*"),array("","","","","",""),$tmaformula));
					if($hdn_tmaid){
						foreach($hdn_tmaid as $t){
							if($t){
								$fim = strpos($t,"}");
								if(!strstr(substr($t,0,$fim),"_")){
									$arrTema[] = substr($t,0,$fim);
								}
							}
						}
					}
					$hdn_indid = explode("{Indicador:",str_replace(array("{Tema:"," ","-","+","*"),array("","","","",""),$tmaformula));
					if($hdn_indid){
							foreach($hdn_indid as $i){
								if($i){
									$fim = strpos($i,"}");
									$valor = substr($i,0,$fim);
									$dado = explode("_",$valor);
									$sql = "select dpeid from painel.detalheperiodicidade where dpedsc = '".$dado[1]."'";
									$dpeid__ = $db->pegaUm($sql);
									if($dpeid__){
										$arrInd[] = $dado[0]."_".$dpeid__;
									}
								}
							}
						}
				}
				?>
				<input type="hidden" id="hdn_tmaid" name="hdn_tmaid" value="<?php echo $arrTema ? implode(",",$arrTema) : "" ?>" />
				<input type="hidden" id="hdn_indid" name="hdn_indid" value="<?php echo $arrInd ? implode(",",$arrInd) : "" ?>" />
				<div id="div_testa_formula" ></div>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Detalhes da Fórmula:</td>
			<td>
				<div id="div_detalhe_formula" >
				<?php
				if($arrTema){
					$arrTemasUsados = array();
					foreach($arrTema as $tma){
						if(is_numeric($tma) && !in_array($tma,$arrTemasUsados)){
							$sql = "select tmadsc from mapa.tema where tmaid = '$tma'";
							$tma_tmadsc = $db->pegaUm($sql);
							echo "<p><b>Tema: $tma</b> - $tma_tmadsc</p>";
							$arrTemasUsados[] = $tma;
						}
					}
				}
				if($arrInd){
					$arrIndUsados = array();
					foreach($arrInd as $ind){
						if(strstr($ind,"_") && !in_array($ind,$arrIndUsados)){
							$data = explode("_",$ind);
							$ind_ = $data[0];
							$dpeid_ = $data[1];
							$sql = "select 
										indnome
									from 
										painel.indicador ind
									where 
										indid = '$ind_'";
							$indnome = $db->pegaUm($sql);
							$sql = "select 
										dpedsc
									from 
										painel.detalheperiodicidade
									where 
										dpeid = '$dpeid_'";
							$dpedsc = $db->pegaUm($sql);
							echo "<p><b>Indicador: $ind_</b> - $indnome de $dpedsc</p>";
							$arrIndUsados[] = $ind;
						}
					}
				}
				?>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubtituloTabela" ></td>
			<td class="SubtituloTabela" >
				<input type="button" id="btn_salvar" onclick="salvarTemaMapa()" value="Salvar" />
				<input type="button" id="btn_cancelar" onclick="window.location.href=window.location" value="Cancelar" />
			</td>
		</tr>
	</table>
</form>
<?php
$sql = "select
			'<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarFormulaMapa(' || tmaid || ')\" /> 
			<img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"excluirFormulaMapa(' || tmaid || ')\" /> ' as acao,
			tmadsc as tema,
			tmaformula as formula
		from
			mapa.tema
		where
			tpdid =".TIPO_MAPA_FORMULA."
		and
			tmastatus = 'A'
		order by
			tmadsc";
$arrCab = array("Ação","Tema","Fórmula");
//dbg($sql,1);
$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
?>