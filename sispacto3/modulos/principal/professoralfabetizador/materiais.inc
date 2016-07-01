<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function gravarMateriaisProfessor(foto) {
	
	var recebeumaterialpacto    = jQuery("[name^='recebeumaterialpacto']:enabled:checked").length;
	if(recebeumaterialpacto==0) {
		alert('Marque se recebeu o material da formação do Pacto.');
		return false;
	}
	var criadocantinholeitura   = jQuery("[name^='criadocantinholeitura']:enabled:checked").length;
	if(criadocantinholeitura==0) {
		alert('Marque se foi criado um cantinho de leitura em sala de aula de alfabetização com o material do PNBE.');
		return false;
	}
	
	if(foto=='F') {
		if(jQuery('#arquivo').val()=='') {
			alert('Selecione a foto');
			return false;
		}
	}
	divCarregando();
	
	jQuery('#salvar').attr('disabled','disabled');
	jQuery('#inserirfoto').attr('disabled','disabled');
	
	document.getElementById('formulario').submit();
}

function criadoCantinhoLeitura(codigo) {
	if(codigo=='1') {
		jQuery('#fotos').css('display','');
	} else {
		jQuery('#fotos').css('display','none');
	}
}

function excluirMateriaisProfessoresFoto(mpfid) {
	var conf = confirm('Deseja realmente remover esta foto?');
	
	if(conf) {
		window.location='sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&requisicao=excluirMateriaisProfessoresFoto&mpfid='+mpfid;
	}

}

jQuery(document).ready(function() {
<? if($consulta_pfl) : ?>
jQuery("#salvarcontinuar").css('display','none');
jQuery("#salvar").css('display','none');
jQuery("[name^='recebeumaterialpacto'],[name^='recebeumaterialpnld'],[name^='recebeulivrospnld'],[name^='recebeumaterialpnbe'],[name^='criadocantinholeitura'],[name^='arquivo']").attr('disabled','disabled');
<? endif; ?>

});


</script>
<?

$materiais = $db->pegaLinha("SELECT * FROM sispacto3.materiaisprofessores WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
if($materiais) extract($materiais);

?>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gerenciarMateriaisProfessores">
<input type="hidden" name="mapid" value="<?=$db->pegaUm("SELECT mapid FROM sispacto3.materiaisprofessores WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'") ?>">
<input type="hidden" name="iusd" value="<?=$_SESSION['sispacto3']['professoralfabetizador']['iusd'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Materiais</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&rfuparcela=".$rfuparcela); ?></td>
	</tr>
	
		<tr>
		<td colspan="2">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloDireita">Professor, você recebeu o material de formação do Pacto da Matemática?</td>
				<td><input type="radio" name="recebeumaterialpacto" value="1" <?=(($recebeumaterialpacto=="1")?"checked":"") ?>> Sim, recebi o material fornecido pelo MEC<br> <input type="radio" name="recebeumaterialpacto" value="2" <?=(($recebeumaterialpacto=="2")?"checked":"") ?>> Sim, recebi uma cópia do material providenciada pelo município<br> <input type="radio" name="recebeumaterialpacto" value="3" <?=(($recebeumaterialpacto=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Na turma da qual você é regente, há um cantinho de leitura para alfabetização com o material do PNBE de 2013 e 2014?</td>
				<td><input type="radio" name="criadocantinholeitura" value="1" onclick="if(this.checked){criadoCantinhoLeitura(this.value);}" <?=(($criadocantinholeitura=="1")?"checked":"") ?>> Sim, criamos o cantinho de leitura<br> <input type="radio" name="criadocantinholeitura" value="3" onclick="if(this.checked){criadoCantinhoLeitura(this.value);}" <?=(($criadocantinholeitura=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr id="fotos" <?=(($criadocantinholeitura=="1")?"":"style=display:none;") ?>>
				<td colspan="2" class="SubTituloCentro">
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="SubTituloDireita">Selecione arquivo</td>
					<td><input type="file" name="arquivo" id="arquivo"></td>
					<td class="SubTituloDireita">Descrição</td>
					<td><?=campo_textarea('mapdsc', 'N', 'S', '', '70', '4', '250'); ?></td>
					<td class="SubTituloCentro"><input type="button" id="inserirfoto" value="Inserir Foto" onclick="gravarMateriaisProfessor('F');"></td>
				</tr>
				</table>
				<br/>
				<br/>
				<fieldset><legend>Fotos</legend>
				<?
				echo "<table>";
				echo "<tr>";
				if($mapid) {
					$_SESSION['imgparams']['tabela'] = "sispacto3.materiaisprofessoresfotos";
					$_SESSION['imgparams']['filtro'] = "cnt.mapid='".$mapid."'";
					$sql = "SELECT * FROM sispacto3.materiaisprofessoresfotos WHERE mapid='".$mapid."'";
					$fotos = $db->carregar($sql);
					if($fotos) {
						foreach($fotos as $ft) {
							echo "<td align=\"center\"><img id=".$ft['arqid']." src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sispacto3','imagem','width=850,height=600,resizable=yes');\"><br><input type=\"button\" name=\"excluirfoto\" value=\"Excluir\" onclick=\"excluirMateriaisProfessoresFoto('".$ft['mpfid']."');\"></td>";
						}
					} else {
						echo "<td>Não existem fotos cadastradas</td>";
					}
				} else {
					echo "<td>Não existem fotos cadastradas</td>";
				}
				echo "</tr>";
				echo "</table>";
				?>
				</fieldset>
				</td>
			</tr>
			</table>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Salvar" id="salvar" onclick="gravarMateriaisProfessor('');"> 
		</td>
	</tr>
</table>
</form>
