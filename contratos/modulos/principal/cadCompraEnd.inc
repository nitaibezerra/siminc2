<?php
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "evento/classes/CoendereCoentrega.class.inc";

verificaSessao(true);

$boAlterar = true;

$sql = "select 
			coeid
	     FROM evento.coenderecoentrega  ee 
	     	inner join evento.coadesao a on a.coaid = ee.coaid
	     where a.copid = ". $_SESSION['copid'] ." and a.usgid = ". $_SESSION['unidade'];
$boEndereco = $db->pegaUm($sql);

if($boEndereco){
	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
	$boAlterar = permissaoAlterar($coaid);
}

if( $_POST['ajaxestuf'] ){	 
	header('content-type: text/html; charset=ISO-8859-1');
 
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod_", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'muncod_' ));
}

if($_POST['formulario']){
	extract($_POST);
	
	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
	if(!$coaid){
		echo "<script>
				alert('Não existe Adesão cadastrada para esta unidade.');
				history.back(-1);
		  	</script>";
		die;
	}
	
	$medlatitude  = implode(".", $medlatitude);
	$medlongitude = implode(".", $medlongitude);
	
	$obCoendereCoentrega = new CoendereCoentrega();
	$obCoendereCoentrega->coeid    		= $coeid;
	$obCoendereCoentrega->coenddsc 		= $coenddsc;
	$obCoendereCoentrega->coaid    	  	= $coaid;
	$obCoendereCoentrega->muncod   		= $muncod_;
	$obCoendereCoentrega->estuf    		= $estuf;
	$obCoendereCoentrega->coendcep 		= str_replace(array(".","-"),"",$endcep);
	$obCoendereCoentrega->coendlog 		= $endlog;
	$obCoendereCoentrega->coendcom		= $endcom;
	$obCoendereCoentrega->coendbai 		= $endbai;
	$obCoendereCoentrega->coendnum 		= $endnum;
	$obCoendereCoentrega->coendhrini 	= $coendhrini;
	$obCoendereCoentrega->coendhrfim 	= $coendhrfim;
	$obCoendereCoentrega->coendrespalmx = $coendrespalmx;
	$obCoendereCoentrega->medlatitude 	= ($medlatitude) ? $medlatitude : null;
	$obCoendereCoentrega->medlongitude 	= ($medlongitude) ? $medlongitude : null;
	$obCoendereCoentrega->endzoom 		= ($endzoom) ? $endzoom : null;
	$obCoendereCoentrega->coendstatus 	= 'A';
	
	$obCoendereCoentrega->salvar();
	$obCoendereCoentrega->commit();
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadCompraEnd","&coeid=".$obCoendereCoentrega->coeid);
	unset($obCoendereCoentrega);
	die;
}
if($_GET['coeidExcluir']){
	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
	if(!$coaid){
		echo "<script>
				alert('Não existe Adesão cadastrada para esta unidade.');
				history.back(-1);
		  	</script>";
		die;
	}
	$sql = "select cdiid from evento.codemandaitem where coeid = ".$_GET['coeidExcluir'] ." and coaid = $coaid";
	$cdiid = $db->pegaUm($sql);
	if($cdiid){
		echo "<script>
				alert('Este endereço não pode ser excluído, pois existem itens vinculados a ele, exclua a demanda primeiro.');
				history.back(-1);
		  	</script>";
		die;
	}
	
	$obCoendereCoentrega = new CoendereCoentrega();
	$obCoendereCoentrega->excluir($_GET['coeidExcluir']);
	$obCoendereCoentrega->commit();
	unset($_GET['coeidExcluir']);
	unset($obCoendereCoentrega);
	$db->sucesso("principal/cadCompraEnd");
	die;
}

$obCoendereCoentrega = new CoendereCoentrega($_GET['coeid']);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoUnidade($_SESSION['unidade']); 
montaCabecalhoProcesso($_SESSION['copid'], false); 
?>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidadesn.js"></script>
<script type="text/javascript">
<!--
    function excluirEnderecoEntrega(coeid){
		if(confirm('Deseja excluir registro')){
			window.location.href = "/evento/evento.php?modulo=principal/cadCompraEnd&acao=A&coeidExcluir="+coeid;
			return true;
		} else {
			return false;
		}
	}
	
    function salvaEnd(){
	 	var nomeform 		= 'formulario';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0]			= "coenddsc";
		campos[1]			= "coendrespalmx";   
		campos[2]			= "coendhrini";   
		campos[3]			= "coendhrfim";   
		campos[4] 			= "endcep";
		campos[5]			= "endlog"; 
		campos[6]			= "endbai";   
		campos[7]			= "endnum";
		campos[8] 			= "estuf";
		campos[9] 			= "muncod_";
		
		tiposDeCampos[0] 	= "texto";
		tiposDeCampos[1] 	= "texto"; 
		tiposDeCampos[2] 	= "texto";
		tiposDeCampos[3] 	= "texto";
		tiposDeCampos[4] 	= "texto";
		tiposDeCampos[5] 	= "texto";
		tiposDeCampos[6] 	= "texto";
		tiposDeCampos[7] 	= "texto";
		tiposDeCampos[8] 	= "select";
		tiposDeCampos[9] 	= "select";
		
		validaForm(nomeform, campos, tiposDeCampos, submeterForm );
	}
	
	function filtraTipo(estuf) {
		if( !estuf ){
			return false;
		}
		td 	   = document.getElementById('municipio');
		select = document.getElementsByName('muncod_')[0];
		
		if (select){
			select.disabled = true;
			select.options[0].text = 'Aguarde...';
			select.options[0].selected = true;
		}	
		
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('evento.php?modulo=principal/cadCompraEnd&acao=A', {
								        method:     'post',
								        parameters: '&ajaxestuf=' + estuf,
								        asynchronous: true,
								        onComplete: function (res)
								        {			  
											td.innerHTML = res.responseText;
											td.style.visibility = 'visible';
											//alteraComboMuncod();
								        }
								  });
		// Espera 100 milisegundos para dar tempo da função AJAX ser executada.
		window.setTimeout('alteraComboMuncod()', 1000);
		
	}
	
	function alteraComboMuncod(){
		var muncod = $('muncod').value;
		if(muncod){
			var comboMunicipio = document.getElementById('muncod_');
			for (var i = 0; i < comboMunicipio.length; i++) {
				var indiceCombo = comboMunicipio.options[i].index;
				var textoCombo = comboMunicipio.options[i].text;
				var valorCombo = comboMunicipio.options[i].value;
				
				if(valorCombo == muncod){
					comboMunicipio.options[i].selected = true;
				}
			}
		}
	}
	
</script>
<form method="post" name="formulario" id="formulario" action="/evento/evento.php?modulo=principal/cadCompraEnd&acao=A">
<input type="hidden" name="muncod" id="muncod" class="CampoEstilo" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="font-weight: bold" colspan="2">
					Dados de Entrega
			</td>
		</tr>
		<tr>
			<td width=25%  class="SubTituloDireita" >
					Nome do local de entrega:
			</td>
			<td id="tdEntdsc">
			<input type="text" name="coenddsc" class="CampoEstilo" title="Local de entrega" id="coenddsc" value="<?php echo $obCoendereCoentrega->coenddsc; ?>" size="100" maxlength="200" /> <img src="../imagens/obrig.gif" />
			<input type="hidden" name="entid" id="entid" value="" />
			<input type="hidden" name="formulario" id="formulario" value="1" />
			<input type="hidden" name="coeid" id="coeid" value="<? echo $obCoendereCoentrega->coeid; ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Responsável pelo recebimento:
			</td>
			<td><?$coendrespalmx = $obCoendereCoentrega->coendrespalmx; ?>
				<?= campo_texto( 'coendrespalmx', 'S', $permissao_formulario, 'Responsável pelo recebimento', 67, 67, '', '','','','','id="coendrespalmx"'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
					Horário de Funcionamento:
			</td>
			<td><?$coendhrini = $obCoendereCoentrega->coendhrini; ?>
				<?= campo_texto( 'coendhrini', 'N', $permissao_formulario, 'Horário de Funcionamento', 5, 5, '##:##', '','','','','id="coendhrini"'); ?>
				 às 
				 <?$coendhrfim = $obCoendereCoentrega->coendhrfim; ?>
				<?= campo_texto( 'coendhrfim', 'S', $permissao_formulario, 'Horário de Funcionamento', 5, 5, '##:##', '','','','','id="coendhrfim"'); ?>
				horas
			</td>
		<tr>
	<?php
               
        $endereco = new Endereco();
        $entidade->enderecos[0] = $endereco;

        $mundescricao = "";
		if($obCoendereCoentrega->muncod){
           $mundescricao = $obCoendereCoentrega->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$obCoendereCoentrega->muncod}'");
        }
        ?>
                
		<tr>
			<td style="font-weight: bold" colspan="2">Endereço</td>
		</tr>
		
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap"><label>CEP:</label></td>
			<td>
				<input type="text" name="endcep" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);" onblur="getEnderecoPeloCEP(this.value,''); filtraTipo($('estuf').value); " class="CampoEstilo" id="endcep" value="<?php echo $obCoendereCoentrega->coendcep; ?>" size="13" maxlength="10" /> <img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>
			<td>
				<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog" value="<?php echo $obCoendereCoentrega->coendlog; ?>" size="48" />
			</td>
		</tr>
		
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
			<td>
				<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum" value="<?php echo $obCoendereCoentrega->coendnum; ?>" size="5" maxlength="8" onkeypress="return somenteNumeros(event);" />
			</td>
		</tr>
		
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
			<td>
				<input type="text" name="endcom" class="CampoEstilo" id="endcom" value="<?php echo $obCoendereCoentrega->coendcom; ?>" size="48" maxlength="100" />
			</td>
		</tr>
		
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
			<td>
				<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai" value="<?php echo $obCoendereCoentrega->coendbai; ?>" />
			</td>
		</tr>
		
		<tr id="tr_estado">
		<td class = "subtitulodireita"> 
			 Estado:
		</td>
		<td>
		<?
			 $estuf = $obCoendereCoentrega->estuf;
			 $sql = "select
					 e.estuf as codigo, e.estdescricao as descricao 
					from
					 territorios.estado e 
					order by
					 e.estdescricao asc";
			 $db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf',false,null,'Estado');
			 ?>						
		</td>
		</tr>
		<tr id="tr_municipio" >
		<td class = "subtitulodireita">
				Município:
				<br/>
			</td>
			<td  id="municipio">
				<?
				if ($obCoendereCoentrega->estuf) {
					$sql = "select
							 muncod as codigo, 
							 mundescricao as descricao 
							from
							 territorios.municipio
							where
							 estuf = '".$obCoendereCoentrega->estuf."' 
							order by
							 mundescricao asc";
					$muncod_ = $obCoendereCoentrega->muncod;
					$db->monta_combo( "muncod_", $sql, 'S', 'Selecione...', '', '', '','','S', 'muncod_',false,null,'Município');
				} else {
					$db->monta_combo( "muncod_", array(), 'S', 'Selecione o Estado', '', '', '', '', 'S', 'muncod_',false,null,'Município');				
				}
				?>
				<input readonly="readonly" type="hidden" name="mundescricao" class="CampoEstilo" id="mundescricao" value="" />	
			</td>	
		</tr>
		<!-- tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
		<td>
		<input readonly="readonly" type="text" name="mundescricao" class="CampoEstilo" id="mundescricao" value="' . $mundescricao . '" /> 
		<input type="hidden" name="muncod" id="muncod" class="CampoEstilo" value="' . $obCoendereCoentrega->muncod . '" />
		<input readonly="readonly" type="text" name="estuf" class="CampoEstilo" id="estuf" value="' . $obCoendereCoentrega->estuf . '" style="width: 5ex; padding-left: 2px" />
		</td>
		</tr -->
		<script> document.getElementById('endcep').value = mascaraglobal('##.###-###', document.getElementById('endcep').value);</script>
		<?php                
		$medlatitude = explode('.',$obCoendereCoentrega->medlatitude);
		$medlongitude = explode('.',$obCoendereCoentrega->medlongitude);
		?>
		<tr>
			<td class="SubTituloDireita">Latitude :</td><td>
				<input name="medlatitude[0]" id="graulatitude" maxlength="2" size="3" value="<? echo $medlatitude[0]; ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" type="text"> º 
				<input name="medlatitude[1]" id="minlatitude" size="3" maxlength="2" value="<? echo $medlatitude[1]; ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" type="text"> ' 
				<input name="medlatitude[2]" id="seglatitude" size="3" maxlength="2" value="<? echo $medlatitude[2]; ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" type="text"> " 
				<select name="medlatitude[3]" id="pololatitude" class="CampoEstilo" style="width: 50px;">
					<?php
					$selectS = "";
					$selectN = "";
					
					if(trim($medlatitude[3]) == "S"){
						$selectS = "selected";	
					} else {
						$selectN = "selected";						
					}
					?>
					<option value="S" <?echo $selectS ?> >S</option><option <?echo $selectN ?> value="N">N</option>
				</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude :</td><td>
				<input name="medlongitude[0]" id="graulongitude" maxlength="2" size="3" value="<? echo $medlongitude[0]; ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" type="text"> º 
				<input name="medlongitude[1]" id="minlongitude" size="3" maxlength="2" value="<? echo $medlongitude[1]; ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" type="text"> ' 
				<input name="medlongitude[2]" id="seglongitude" size="3" maxlength="2" value="<? echo $medlongitude[2]; ?>" class="normal" onkeyup="this.value=mascaraglobal('##',this.value);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" type="text"> " 
				<select name="medlongitude[3]" id="pololongitude" class="CampoEstilo" style="width: 50px;">
					<?php
					$selectW = "";
					$selectE = "";
					
					if(trim($medlongitude[3]) == "S"){
						$selectW = "selected";	
					} else {
						$selectE = "selected";						
					}
					?>
					<option value="W" <?echo $selectW ?> >W</option><option value="E" <?echo $selectE ?> >E</option>
				</select>
				<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		<tr><td class="SubTituloDireita">&nbsp;</td><td><a href="#" onclick="abreMapaEntidade('');">Visualizar / Buscar No Mapa</a> <input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text"></td></tr><tr>
	</table>
	<div id=buttonAcao>
	<?php if($boAlterar){ ?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					<input type="button" onclick="salvaEnd()" value="Salvar"  /> <input type="button" value="Incluir Novo" onclick="window.location.href='evento.php?modulo=principal/cadCompraEnd&acao=A';" />
				</td>
			</tr>
	</table>
	<?php } ?>
	</div>
</form>
<?php
if($boAlterar){
	$linhaExcluir = "&nbsp;<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" onclick=\"excluirEnderecoEntrega(' || ee.coeid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'";
} else {
	$linhaExcluir = "&nbsp;<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\">'";
}

$sql = "select 
			'<a style=\"margin: 0 -5px 0 5px;\" href=\"evento.php?modulo=principal/cadCompraEnd&acao=A&coeid='|| ee.coeid ||'\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
			 $linhaExcluir as acao,
			'<a style=\"margin: 0 -5px 0 5px;\" href=\"evento.php?modulo=principal/cadCompraEnd&acao=A&coeid='|| ee.coeid ||'\">' || ee.coenddsc ||'</a>' as nome,
			ee.coendrespalmx as responsavel,
			ee.coendhrini || ' às ' || ee.coendhrfim as horario,
			ee.coendlog  || ', ' || ee.coendnum || ' ' || ee.coendcom || ', ' || ee.coendbai as endereco
	     FROM evento.coenderecoentrega  ee 
	     	inner join evento.coadesao a on a.coaid = ee.coaid
	     where a.copid = ". $_SESSION['copid'] ." and a.usgid = ". $_SESSION['unidade'];
$arUnidades = $db->carregar($sql);
$cabecalho = array("Ação","Nome do Local","Responsável","Horário de Funcionamento","Endereço");
$db->monta_lista_array($arUnidades, $cabecalho, 50, 20, '', 'center','');
echo "<br />";
?>