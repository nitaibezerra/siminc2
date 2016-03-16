<?php
monta_titulo( "Escola", '&nbsp' );
$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo2013_vars']['usucpfdiretor']);
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

	jQuery(function() {
	
		var UA = navigator.userAgent;
		if (UA.indexOf('MSIE') > -1) {
		    alert('Algumas funcionalidades podem não funcionar corretamente com este navegar.\nRecomendamos que você utilize o Mozilla Firefox!');
		}
			
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
	
		if( jQuery('[name=pdecep]').val() ){
				jQuery('[name=pdecep]').val( mascaraglobal('###.###-##', jQuery('[name=pdecep]').val() ) );
			}
		
		});
		jQuery('[name=pdeemail]').blur( function() {
			validaEmail();
		});
		
	
	function confirmarDadosEscola(local)
	{
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		if( erro == 0){
			if( !validaEmail() ){
				erro = 1;
			}
		}

		if(erro == 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("#form_cad_escola").submit();
		}
	}
	
	function validaEmail()
	{
		var sEmail	= jQuery('[name=pdeemail]').val();
		var emailFilter = /^.+@.+\..{2,}$/;
		var illegalChars = /[\(\)\<\>\,\;\:\\\/\"\[\]]/
		if( !(emailFilter.test(sEmail)) || sEmail.match(illegalChars) ){
			alert('Por favor, informe um email válido.');
			jQuery('[name=pdeemail]').focus();
			return false;
		}else{
			return true;
		}
	}
	
	function abreMapa(){
		
		var muncod = window.document.getElementById("muncod").value;
	
		var graulatitude = window.document.getElementById("graulatitude").value;
		var minlatitude  = window.document.getElementById("minlatitude").value;
		var seglatitude  = window.document.getElementById("seglatitude").value;
		var pololatitude = window.document.getElementById("pololatitude_").innerHTML;
		
		var graulongitude = window.document.getElementById("graulongitude").value;
		var minlongitude  = window.document.getElementById("minlongitude").value;
		var seglongitude  = window.document.getElementById("seglongitude").value;
		
		var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
		var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
		
		var janela=window.open('?modulo=principal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
	
	}
		
	
</script>
<link rel="stylesheet" type="text/css" href="../pdeinterativo2013/css/pdeinterativo2013.css"/>
<form name="form_cad_escola" id="form_cad_escola"  method="post" action="" >
	<input type="hidden" name="requisicao" value="confirmaDadosEscola" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita blue" >Orientações</td>
			<td class="SubTituloEsquerda blue normal" >Os dados abaixo foram extraídos do Censo Escolar, todavia, os campos com o símbolo ( <?php echo obrigatorio() ?> ) são obrigatórios e podem ser atualizados. <br /><b>Lembre-se: </b>as mudanças inseridas neste sistema não alteram os dados oficiais do Censo Escolar e serão utilizados apenas para elaborar o PDE Escola.</td>
		</tr>
		<tr>
			<td width="25%" class="SubTituloDireita" >Código da Escola (INEP):</td>
			<td><?php echo campo_texto("pdicodinep","N","N","Código da Escola (INEP)","14","14","[#]","","","","","","",$arrDados['pdicodinep'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Escola:</td>
			<td id="pdenome"><?php echo $arrDados['pdenome'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Esfera:</td>
			<td><?php echo $arrDados['pdiesfera'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Localização:</td>
			<td><?php echo $arrDados['pdilocalizacao'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Modalidades Oferecidas:</td>
			<td>
				<?php $arrModalidades = $arrDados['pdicodinep'] ? recuperaModalidadesEnsinoPorCodigoINEP($arrDados['pdicodinep']) : array() ; ?>
				<?php echo ($arrModalidades ? implode(", ",$arrModalidades)."." : "N/A") ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Ciclo:</td>
			<td><?php echo $arrDados['ciclo'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Níveis Oferecidos:</td>
			<td>
				<?php $arrNiveis = $arrDados['pdicodinep'] ? recuperaNiveisEnsinoPorCodigoINEP($arrDados['pdicodinep']) : array() ; ?>
				<?php $cabecalho = array("Nível","Quantidade de Matrículas") ?>
				<?php $db->monta_lista_simples((($arrNiveis[0])?$arrNiveis:array()),$cabecalho,100,10,"S","95%","N","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >CEP:</td>
			<td>
				<?php echo campo_texto("pdecep","S","S","CEP","12","10","###.###-##","","","","","id='pdecep'","",$arrDados['pdecep'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Endereço:</td>
			<td>
				<?php echo campo_texto("pdelogradouro","S","S","Endereço","60","180","","","","","","id='pdelogradouro'","",$arrDados['pdelogradouro'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Número:</td>
			<td>
				<?php echo campo_texto("pdinumero","N","S","Número","14","100","","","","","","id='pdinumero'","",$arrDados['pdinumero'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Complemento:</td>
			<td>
				<?php echo campo_texto("pdecomplemento","N","S","Complemento","20","100","","","","","","id='pdecomplemento'","",$arrDados['pdecomplemento'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Bairro:</td>
			<td>
				<?php echo campo_texto("pdebairro","N","S","Bairro","40","100","","","","","","id='pdebairro'","",$arrDados['pdebairro'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >UF:</td>
			<td>
				<?php echo campo_texto("estuf","N","N","UF","3","2","","","","","","id='estuf'","",$arrDados['estuf'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td>
				<?php echo campo_texto("mundescricao","N","N","Município","60","180","","","","","","id='mundescricao'","",$arrDados['mundescricao'])?>
				<input type="hidden" name="muncod" id="muncod" value="<?php echo $arrDados['muncod'] ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Possui Energia Elétrica (Censo):</td>
			<td>
			<b>
			<? 
			if($arrDados['pdienergiaeletricacenso']=='t') echo "Sim";
			elseif($arrDados['pdienergiaeletricacenso']=='f') echo "Não";
			else echo "Não informado";
			?>
			</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Possui Energia Elétrica:</td>
			<td><input type="radio" name="pdienergiaeletrica" value="true" <?=(($arrDados['pdienergiaeletrica']=="t")?"checked":"") ?> > Sim <input type="radio" name="pdienergiaeletrica" value="false" <?=(($arrDados['pdienergiaeletrica']=="f")?"checked":"") ?> > Não</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Site / Blog da Escola:</td>
			<td><?php echo campo_texto("pdisite","N","S","Site","60","180","","","","","","id='pdisite'","",$arrDados['pdisite'])?></td>
		</tr>
		<tr>
			<td colspan="2"class="SubTituloTabela" >Coordenadas Geográficas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Latitude</td>
			<td><?php 
			$medlatitude = $arrDados['medlatitude'];
			$latitude = explode(".", $medlatitude);
			$graulatitude = $latitude[0];
			$minlatitude = $latitude[1];
			$seglatitude = $latitude[2];
			$pololatitude = $latitude[3];
			
				
			?> <?= campo_texto( 'graulatitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
			° <?= campo_texto( 'minlatitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
			' <?= campo_texto( 'seglatitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
			'' <input type="hidden" name="pololatitude" id="pololatitude">
			<?php 
			if (trim($pololatitude) == "S") {
				echo "&nbsp;<span id=pololatitude_>S</span>"; 
			} elseif(trim($pololatitude) == "N") {
				echo "&nbsp;<span id=pololatitude_>N</span>";
			} else {
				echo "&nbsp;<span id=pololatitude_></span>";
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude</td>
			<td><?php 
			$medlongitude = $arrDados['medlongitude'];
			$longitude = explode(".", $medlongitude);
			$graulongitude = $longitude[0];
			$minlongitude = $longitude[1];
			$seglongitude = $longitude[2];
			?> <?= campo_texto( 'graulongitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
			° <?= campo_texto( 'minlongitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
			' <?= campo_texto( 'seglongitude', 'N', 'N', '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?>
			'' &nbsp W </td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td><a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
			<input style="display: none;" type="text" name="endereco[endzoom]"
				id="endzoom"
				value=<? if ($arrDados['endzoom'] == null) echo "15"; else echo $arrDados['endzoom'];?>>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Telefone:</td>
			<td id="td_usufone" >
				<?php echo campo_texto("pdidddtelefone","N","S","DDD","2","2","","","","","","","",$arrDados['pdidddtelefone'])?>&nbsp;
				<?php echo campo_texto("pdinumtelefone","S","S","DDD","10","9","####-####","","","","","","",$arrDados['pdinumtelefone'])?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >E-mail da Escola:</td>
			<td><?php echo campo_texto("pdeemail","S","S","E-mail","40","40","","","","","","","",$arrDados['pdeemail'])?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo2013.php?modulo=principal/identificacao&acao=A&aba=Diretor'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="confirmarDadosEscola()">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="confirmarDadosEscola('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo2013.php?modulo=principal/identificacao&acao=A&aba=Galeria'" >
			</td>
		</tr>
	</table>
</form>