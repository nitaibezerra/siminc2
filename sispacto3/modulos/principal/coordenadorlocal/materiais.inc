<?
$consulta = verificaPermissao();

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['docidturma'] );

if($estado['esdid'] == ESD_FECHADO_TURMA) {
	$consulta = true;
}

$perfis = pegaPerfilGeral();

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function carregarAlunosTurma() {
	ajaxatualizar('requisicao=carregarAlunosTurma&turid=<?=$_REQUEST['turid'] ?>','td_alunosturmas');
}

function abrirAlunosTurma(turid) {
	window.open('sispacto.php?modulo=principal/coordenadorlocal/inserirprofessoresturmas&acao=A&turid='+turid,'Turmas','scrollbars=yes,height=600,width=1100,status=no,toolbar=no,menubar=no,location=no');
}


function comporTurma(turid) {
	divCarregando();
	window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas&turid='+turid;
}




function gravarMateriais(foto) {
	
	var recebeumaterialpacto    = jQuery("[name^='recebeumaterialpacto']:enabled:checked").length;
	if(recebeumaterialpacto==0) {
		alert('Marque se recebeu o material da formação do Pacto.');
		return false;
	}
	var distribuiumaterialpacto = jQuery("[name^='distribuiumaterialpacto']:enabled:checked").length;
	if(distribuiumaterialpacto==0) {
		alert('Marque se distribuiu entre orientadores de estudo e professores alfabetizadores o material da formação do Pacto.');
		return false;
	}
	var recebeumaterialpnld     = jQuery("[name^='recebeumaterialpnld']:enabled:checked").length;
	if(recebeumaterialpnld==0) {
		alert('Marque se recebeu o material referente ao Programa Nacional do Livro Didático (PNLD) em cada escola.');
		return false;
	}
	var recebeulivrospnld       = jQuery("[name^='recebeulivrospnld']:enabled:checked").length;
	if(recebeulivrospnld==0) {
		alert('Marque se recebeu os livros do Programa Nacional Biblioteca da Escola (PNBE) específico para cada sala de aula de alfabetização.');
		return false;
	}
	var criadocantinholeitura   = jQuery("[name^='criadocantinholeitura']:enabled:checked").length;
	if(criadocantinholeitura==0) {
		alert('Marque se foi criado um cantinho de leitura em cada sala de aula de alfabetização com o material do PNBE.');
		return false;
	}
	
	if(foto=='F') {
		if(jQuery('#arquivo').val()=='') {
			alert('Selecione a foto');
			return false;
		}
	}
	
	document.getElementById('formulario').submit();
}

function criadoCantinhoLeitura(codigo) {
	if(codigo=='1' || codigo=='2') {
		jQuery('#fotos').css('display','');
	} else {
		jQuery('#fotos').css('display','none');
	}
}

jQuery(document).ready(function() {
});


</script>
<?

$materiais = $db->pegaLinha("SELECT * FROM sispacto.materiais WHERE picid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']."'");
if($materiais) extract($materiais);

?>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gerenciarMateriais">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Turmas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
		<tr>
		<td colspan="2">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloDireita">O seu estado ou município recebeu o material da formação do Pacto?</td>
				<td><input type="radio" name="recebeumaterialpacto" value="1" <?=(($recebeumaterialpacto=="1")?"checked":"") ?>> Sim, totalmente <input type="radio" name="recebeumaterialpacto" value="2" <?=(($recebeumaterialpacto=="2")?"checked":"") ?>> Sim, parcialmente <input type="radio" name="recebeumaterialpacto" value="3" <?=(($recebeumaterialpacto=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">O seu estado ou município distribuiu entre orientadores de estudo e professores alfabetizadores o material da formação do Pacto?</td>
				<td><input type="radio" name="distribuiumaterialpacto" value="1" <?=(($distribuiumaterialpacto=="1")?"checked":"") ?>> Sim, totalmente <input type="radio" name="distribuiumaterialpacto" value="2" <?=(($distribuiumaterialpacto=="2")?"checked":"") ?>> Sim, parcialmente <input type="radio" name="distribuiumaterialpacto" value="3" <?=(($distribuiumaterialpacto=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">O seu estado ou município recebeu o material referente ao Programa Nacional do Livro Didático (PNLD) em cada escola</td>
				<td><input type="radio" name="recebeumaterialpnld" value="1" <?=(($recebeumaterialpnld=="1")?"checked":"") ?>> Sim, totalmente <input type="radio" name="recebeumaterialpnld" value="2" <?=(($recebeumaterialpnld=="2")?"checked":"") ?>> Sim, parcialmente <input type="radio" name="recebeumaterialpnld" value="3" <?=(($recebeumaterialpnld=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">O seu estado ou município recebeu os livros do PNLD – Obras Complementares específico para cada sala de aula de alfabetização?</td>
				<td><input type="radio" name="recebeulivrospnld" value="1" <?=(($recebeulivrospnld=="1")?"checked":"") ?>> Sim, totalmente <input type="radio" name="recebeulivrospnld" value="2" <?=(($recebeulivrospnld=="2")?"checked":"") ?>> Sim, parcialmente <input type="radio" name="recebeulivrospnld" value="3" <?=(($recebeulivrospnld=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">O seu estado ou município recebeu os livros do Programa Nacional Biblioteca da Escola (PNBE) específico para cada sala de aula de alfabetização?</td>
				<td><input type="radio" name="recebeumaterialpnbe" value="1" <?=(($recebeumaterialpnbe=="1")?"checked":"") ?>> Sim, totalmente <input type="radio" name="recebeumaterialpnbe" value="2" <?=(($recebeumaterialpnbe=="2")?"checked":"") ?>> Sim, parcialmente <input type="radio" name="recebeumaterialpnbe" value="3" <?=(($recebeumaterialpnbe=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Foi criado um cantinho de leitura em cada sala de aula de alfabetização com o material do PNBE</td>
				<td><input type="radio" name="criadocantinholeitura" value="1" onclick="if(this.checked){criadoCantinhoLeitura(this.value);}" <?=(($criadocantinholeitura=="1")?"checked":"") ?>> Sim, totalmente <input type="radio" name="criadocantinholeitura" value="2" onclick="if(this.checked){criadoCantinhoLeitura(this.value);}" <?=(($criadocantinholeitura=="2")?"checked":"") ?>> Sim, parcialmente <input type="radio" name="criadocantinholeitura" value="3" onclick="if(this.checked){criadoCantinhoLeitura(this.value);}" <?=(($criadocantinholeitura=="3")?"checked":"") ?>> Não</td>
			</tr>
			<tr id="fotos" <?=(($criadocantinholeitura=="1" || $criadocantinholeitura=="2")?"":"style=display:none;") ?>>
				<td colspan="2" class="SubTituloCentro">
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="SubTituloDireita">Selecione arquivo</td>
					<td><input type="file" name="arquivo" id="arquivo"></td>
					<td class="SubTituloDireita">Descrição</td>
					<td><?=campo_textarea('mafdsc', 'N', 'S', '', '70', '4', '250'); ?></td>
					<td class="SubTituloCentro"><input type="button" value="Inserir Foto" onclick="gravarMateriais('F');"></td>
				</tr>
				</table>
				<br/>
				<br/>
				<fieldset><legend>Fotos</legend>
				<?
				echo "<table>";
				echo "<tr>";
				if($matid) {
					$_SESSION['imgparams']['tabela'] = "sispacto.materiaisfotos";
					$_SESSION['imgparams']['filtro'] = "cnt.matid='".$matid."'";
					$sql = "SELECT * FROM sispacto.materiaisfotos WHERE matid='".$matid."'";
					$fotos = $db->carregar($sql);
					if($fotos) {
						foreach($fotos as $ft) {
							echo "<td><img id=".$ft['arqid']." src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sispacto','imagem','width=850,height=600,resizable=yes');\"></td>";
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
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=turmas';"> 
			<input type="button" value="Salvar" onclick="gravarMateriais('');">
		</td>
	</tr>
</table>
</form>
