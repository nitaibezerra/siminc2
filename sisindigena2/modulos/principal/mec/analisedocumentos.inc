<?

function detalhesTroca($dados) {
	global $db;
	$resp = carregarJustificativaTroca(array('jtoid'=>$dados['jtoid']));
	
	?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" class="SubTituloCentro">Pergunta: Por que este Orientador de Estudo será substituído?</td>
			</tr>
			<tr>
				<td><input type="checkbox" disabled <?=((in_array('1',$resp['resp1']))?'checked':'') ?> value="1"></td>
				<td>Porque o próprio Orientador desistiu da função.</td>
			</tr>
			<tr <?=((in_array('1',$resp['resp1']))?'':'style="display:none"') ?>>
				<td colspan="2">
				
				<table cellSpacing="1" cellPadding="3">
					<tr>
						<td colspan="4" class="SubTituloEsquerda">Anexe um documento assinado pelo Orientador de Estudo que será substituído informando as razões da desistência.</td>
					</tr>
					<tr>
						<td colspan="4">
						<? if($resp['arq1']['1']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERe arqid='".$resp['arq1']['1']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq1']['1'] ?>';"> <?=$arquivo ?>
						<? endif; ?>
						</td>
					</tr>
				</table>
				
				</td>
			</tr>
			<tr>
				<td><input type="checkbox" disabled <?=((in_array('2',$resp['resp1']))?'checked':'') ?> value="2"></td>
				<td>Porque o Orientador de Estudo cadastrado não preenche um ou mais requisitos de seleção.</td>
			</tr>
			<tr <?=((in_array('2',$resp['resp1']))?'':'style="display:none"') ?>>
				<td colspan="2">
				<table cellSpacing="1" cellPadding="3">
					<tr>
						<td colspan="4" class="SubTituloEsquerda">Assinale o(s) requisito(s) que o Orientador de Estudo selecionado não cumpre e anexe o(s) respectivo(s) documento(s) comprobatório(s).</td>
					</tr>

					<tr>
						<td><input type="checkbox" disabled <?=((in_array('1',$resp['resp2']))?'checked':'') ?> value="1"></td>
						<td>
						O Orientador selecionado não é mais servidor efetivo da Secretaria de Educação<br/>
						<span <?=((in_array('1',$resp['resp2']))?'':'style="display:none"') ?> id="sp<?=$oe['iusd'] ?>_1"><b>Anexe o documento de exoneração:</b>
						<? if($resp['arq2']['1']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['1']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['1'] ?>';"> <?=$arquivo ?>
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" disabled <?=((in_array('2',$resp['resp2']))?'checked':'') ?> value="2"></td>
						<td>
						O Orientador selecionado não é profissional do magistério<br/>
						<span <?=((in_array('2',$resp['resp2']))?'':'style="display:none"') ?> ><b>Anexe um documento da Secretaria que declare a formação divergente do magistério, com assinatura de “ciente” dada pelo Orientador que será substituído:</b> 
						<? if($resp['arq2']['2']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['2']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['2'] ?>';"> <?=$arquivo ?>
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" disabled <?=((in_array('3',$resp['resp2']))?'checked':'') ?> value="3"></td>
						<td>
						O Orientador selecionado não é formado em Pedagogia nem possui Licenciatura<br/>
						<span <?=((in_array('3',$resp['resp2']))?'':'style="display:none"') ?>><b>Anexe uma declaração da Secretaria sobre a formação divergente da exigida, com assinatura de “ciente” dada pelo Orientador que será substituído:</b> 
						<? if($resp['arq2']['3']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['3']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['3'] ?>';"> <?=$arquivo ?>
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" disabled <?=((in_array('4',$resp['resp2']))?'checked':'') ?> value="4"></td>
						<td>
						O Orientador selecionado não atuou por pelo menos 3 anos nos anos iniciais do Ensino Fundamental<br/>
						<span <?=((in_array('4',$resp['resp2']))?'':'style="display:none"') ?>><b>Anexe uma declaração assinada da Secretaria de Educação atestando este fato, com assinatura de “ciente” dada pelo Orientador que será substituído:</b> 
						<? if($resp['arq2']['4']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['4']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['4'] ?>';"> <?=$arquivo ?>
						<? endif; ?>
						</span>
						</td>
					</tr>
					<tr>
						<td><input type="checkbox" disabled <?=((in_array('5',$resp['resp2']))?'checked':'') ?> value="5"></td>
						<td>
						Outro(s) motivo(s) que descumpre(m) a Portaria 1.458, de 14/12/2012<br/>
						<span <?=((in_array('5',$resp['resp2']))?'':'style="display:none"') ?> >
						<b>Descreva os motivos:</b><br/>
						<? echo campo_textarea( 'jtooutrosdsc'.$oe['iusd'], 'N', 'N', '', '70', '4', '300', '', '', '', '', '', $resp['jtooutrosdsc']); ?>
						<br/>
						<b>Anexe um documento que comprove este fato:</b> 
						<? if($resp['arq2']['5']) : ?>
						<? $arquivo = $db->pegaUm("SELECT arqnome||'.'||arqextensao as arquivo FROM public.arquivo WHERE arqid='".$resp['arq2']['5']."'"); ?>
						<img src="../imagens/anexo.gif" align="absmiddle" style="cursor:pointer;" onclick="window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid=<?=$resp['arq2']['5'] ?>';"> <?=$arquivo ?>
						<? endif; ?>
						</span>
						</td>
					</tr>

				</table>
				</td>
			</tr>
		</table>
		<?
	
}

function alterarSituacaoDocumento($dados) {
	global $db;
	
	$sql = "UPDATE sispacto.identificacaousuario SET iusdocumento=".$dados['sit']." WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();
	
	
}

include_once "_funcoes_coordenadorlocal.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>';

echo "<br>";

if(!$_REQUEST['tipo']) $_REQUEST['tipo'] = "comp";

$menu[] = array("id" => 1, "descricao" => "Documentos comprobatórios", "link" => "sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&tipo=comp");
$menu[] = array("id" => 2, "descricao" => "Documentos da troca", "link" => "sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&tipo=troc");

$abaativa = "sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&tipo=".$_REQUEST['tipo'];

echo montarAbasArray($menu, $abaativa);

monta_titulo( "Lista - Análise de Documentos", "Lista para ser aprovação");

?>
<script>
function downloadDocumentoComprobatorio(arqid) {
	alert(arqid);
	window.location='sispacto.php?modulo=principal/mec/analisedocumentos&acao=A&requisicao=downloadDocumento&arqid='+arqid;
}

function alterarSituacaoDocumento(iusd,obj) {
	var conf = confirm('Deseja realmente alterar a situação?');
	if(conf) {
		ajaxatualizar('requisicao=alterarSituacaoDocumento&iusd='+iusd+'&sit='+obj.value,'tt');
	} else {
		if(obj.value=='TRUE') {
			jQuery('[name^=opt'+iusd+'][value^=FALSE]').attr('checked',true);
		}
		if(obj.value=='FALSE') {
			jQuery('[name^=opt'+iusd+'][value^=TRUE]').attr('checked',true);
		}
	}
	
}

function abrirDetalheTroca(jtoid, obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode;
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'tur_coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=detalhesTroca&jtoid='+jtoid,ncol.id);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}


}
</script>
<div id="tt"></div>
<form method="post" name="formulario" id="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Esfera</td>
	<td><?
	$arres = array(0=>array("codigo"=>"M","descricao"=>"Municipal"),1=>array("codigo"=>"E","descricao"=>"Estadual"));
	$db->monta_combo('esfera', $arres, 'S', 'Selecione', '', '', '', '', 'N', 'uf', '', $_REQUEST['esfera']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('uf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '200', 'N', 'uf', '', $_REQUEST['uf']);
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">
	<? 
	if($_REQUEST['uf']) :
		if(!isset($_REQUEST['muncod_endereco'])) $_REQUEST['muncod_endereco'] = $_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['muncod'];
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['uf']."' ORDER BY mundescricao"; 
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $_REQUEST['muncod_endereco']);
	else: 
		echo "Selecione uma UF";
	endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Orientador de Estudo</td>
	<td><?=campo_texto('orientadorestudo', "S", "S", "Nome", 67, 150, "", "", '', '', 0, 'id="orientadorestudo"', '', $_REQUEST['orientadorestudo']); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Filtrar"> <input type="button" value="Todos" onclick="divCarregando();window.location=window.location;"></td>
</tr>
</table>
</form>
<?

if($_REQUEST['esfera']=='E') {
	$wh[] = "p.estuf is not null";
	
	if($_REQUEST['uf']) {
		$wh[] = "e.estuf='".$_REQUEST['uf']."'";
	}
}
elseif($_REQUEST['esfera']=='M') {
	$wh[] = "p.muncod is not null";
	
	if($_REQUEST['uf']) {
		$wh[] = "m.estuf='".$_REQUEST['uf']."'";
	}
	
	if($_REQUEST['muncod_endereco']) {
		$wh[] = "m.muncod='".$_REQUEST['muncod_endereco']."'";
	}
	
}

if($_REQUEST['orientadorestudo']) {
	$wh[] = "removeacento(i.iusnome) ilike removeacento('%".$_REQUEST['orientadorestudo']."%')";
}

if($_REQUEST['tipo']=='comp') {

	$sql = "select i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal,
				   CASE WHEN p.muncod is null THEN 'Estadual - ' || e.estuf || ' / ' || e.estdescricao
				   		ELSE 'Municipal - ' || m.estuf || ' / ' || m.mundescricao END as descricao,
				   '<img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"downloadDocumentoComprobatorio('||(select MAX(arqid) FROM sispacto.portarianomeacao where iusd=i.iusd and ponstatus='A')||');\">' as anexo, 
				   '<input type=radio name=\"opt'||i.iusd||'\" value=\"TRUE\" onclick=\"alterarSituacaoDocumento('||i.iusd||',this);\" '||CASE WHEN i.iusdocumento=true THEN 'checked' ELSE '' END||'> Regular <input type=radio name=\"opt'||i.iusd||'\" value=\"FALSE\" onclick=\"alterarSituacaoDocumento('||i.iusd||',this);\" '||CASE WHEN i.iusdocumento=false THEN 'checked' ELSE '' END||'> Irregular' as opt
			from sispacto.identificacaousuario i
			inner join sispacto.tipoperfil t ON t.iusd = i.iusd and t.pflcod=".PFL_ORIENTADORESTUDO." 
			inner join sispacto.pactoidadecerta p on p.picid = i.picid 
			left join territorios.municipio m on m.muncod = p.muncod 
			left join territorios.estado e on e.estuf = p.estuf 
			where i.iustipoorientador='profissionaismagisterio' ".(($wh)?"AND ".implode(" AND ",$wh):"")."
			order by i.iusnome";
	
	
	$cabecalho = array("CPF","Orientador de Estudo","Email","Município","Anexo","Situação");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

}


if($_REQUEST['tipo']=='troc') {

	$sql = "select '<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"abrirDetalheTroca('||j.jtoid||', this);\">' as consultar, i.iuscpf, i.iusnome, i.iusemailprincipal, 
				   CASE WHEN p.muncod is null THEN 'Estadual - ' || e.estuf || ' / ' || e.estdescricao
				   		ELSE 'Municipal - ' || m.estuf || ' / ' || m.mundescricao END as descricao,
				   '<input type=radio name=\"opt'||i.iusd||'\" disabled value=\"TRUE\" onclick=\"alterarSituacaoDocumento('||i.iusd||',this);\" '||CASE WHEN i.iusdocumento=true THEN 'checked' ELSE '' END||'> Regular <input type=radio name=\"opt'||i.iusd||'\" disabled value=\"FALSE\" onclick=\"alterarSituacaoDocumento('||i.iusd||',this);\" '||CASE WHEN i.iusdocumento=false THEN 'checked' ELSE '' END||'> Irregular' as opt
	
			from sispacto.justificativatrocaorientador j 
			left join sispacto.identificacaousuario i on i.iusd = j.iusd 
			left join sispacto.pactoidadecerta p on p.picid = i.picid 
			left join territorios.municipio m on m.muncod = p.muncod 
			left join territorios.estado e on e.estuf = p.estuf 
			".(($wh)?"WHERE ".implode(" AND ",$wh):"")."";
	
	
	echo "<p align=center style=color:red><b>ESTA TELA AINDA NÃO PERMITE MARCAR IRREGULARIDADE. APENAS ANALISAR OS CASOS COM PROBLEMAS.</b></p>";
	$cabecalho = array("&nbsp;","CPF","Orientador de Estudo Substituído","Email","Município","Situação");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

}
?>