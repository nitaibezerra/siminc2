<?
verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto3']['coordenadorlocal']['iusd']));

$consulta = verificaPermissao();

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis)) {
	$consulta = true;
}

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_LOCAL) {
	$consulta = true;
}

$ar = array("estuf" 	  => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf'],
			"muncod" 	  => $_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod'],
			"dependencia" => $_SESSION['sispacto3']['esfera']);

$totalalfabetizadores = carregarTotalAlfabetizadores($ar);

$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>
function inserirOrientadorEstudo() {
	window.open('sispacto3.php?modulo=principal/coordenadorlocal/inserirorientadorestudo&acao=A','Orientador','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function importarOrientadoresSispactoAnterior() {

	divCarregando();
	
	ajaxatualizar('requisicao=carregarOrientadoresestudoSispacto2014&picid=<?=$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'] ?>','modalImportarOrientadoresSispactoAnterior');
	
	jQuery("#modalImportarOrientadoresSispactoAnterior").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 600,
        modal: true,
     	close: function(){} 
    });

    divCarregado();
	
}


function salvarOrientadoresEstudos(goto) {

	if(document.getElementById('alimuncodorigem')) {
		if(document.getElementById('alimuncodorigem').value=='') {
			alert('Selecione um município para incluir os professores');
			return false;
		}
	}
	
	var invalidaanexo=false;
	jQuery("[name^='anexoportaria[']").each(function() {
		if(this.value=='') {
			invalidaanexo=true;
		}
	});
	
	if(invalidaanexo) {
		alert('É obrigatório anexar a Portaria que designa os profissionais do magistério indicados como servidores efetivos da Secretaria.');
		return false;	
	}
	
	if((document.getElementById('tabelaorientadoresestudo').rows.length-1)==0) {
		alert('Selecione Orientadores de Estudo');
		return false;
	}
	
	if((document.getElementById('tabelaorientadoresestudo').rows.length-1)<jQuery('#total_a_serem_cadastrados').val()) {
		var conf = confirm("Não foram preenchidas todas as vagas dos orientadores de estudos. Deseja continuar assim mesmo?");
		if(!conf) {
			return false;
		}
	}

	divCarregando();
	
	jQuery('#alteracaodados').val('0');
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function removerAnexoPortaria(ponid) {
	var conf = confirm('Deseja realmente excluir este anexo?');
	
	if(conf) {
		divCarregando();
		window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=removerAnexoPortaria&ponid='+ponid;
	}
}


function removerOrientadorEstudo(iusd, obj) {
	var conf = confirm('Deseja realmente excluir este orientador? Com essa exclusão, todos os professores cadastrados na turma desse orientador de estudo serão removidos.');
	
	if(conf) {
	
		divCarregando();
		
		var apagarLinha = true;
		
		if(iusd) {
			jQuery.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: 'requisicao=removerOrientadorEstudo&iusd='+iusd,
		   		async: false,
		   		success: function(html){
		   		
		   			if(html!='TRUE') {
		   			
		   				apagarLinha = false;
		   				alert(html);
		   				
		   			}
		   		}
			});
			
		}
		
		if(apagarLinha) {
		
			var tabela = obj.parentNode.parentNode.parentNode.parentNode;
			var linha = obj.parentNode.parentNode.parentNode;
			tabela.deleteRow(linha.rowIndex);
			
		}
		
		divCarregado();
	}
	

}


function desejaIncluirProfessoresFormacao(obj) {
	if(obj.value=="TRUE") {
		ajaxatualizar('requisicao=atualizarSelecaoPublica&picid=<?=$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'] ?>&picselecaopublica=FALSE','');
		ajaxatualizar('requisicao=atualizarInclusaoProfessorRede&picid=<?=$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'] ?>&picincluirprofessorrede=TRUE','');
		jQuery('#tr_picselecaopublica').css('display','none');
		jQuery('#tr_orientadoresestudo').css('display','none');
		jQuery('#tr_mm').css('display','');
	} else if(obj.value=="FALSE") {
		ajaxatualizar('requisicao=atualizarInclusaoProfessorRede&picid=<?=$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid'] ?>&picincluirprofessorrede=FALSE','');
		jQuery('#tr_picselecaopublica').css('display','');
		jQuery('#tr_mm').css('display','none');

	
	}
}

function verificarSituacaoAdesao(muncod) {
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A',
   		data: 'requisicao=verificarSituacaoAdesao&muncod='+muncod,
   		async: false,
   		success: function(msg){
   			if(msg!='FALSE') {
   				alert('A rede escolhida já concluiu a indicação dos seus Orientadores de Estudo. Escolha outro município.');
   				jQuery('#alimuncodorigem').val('');
   			}
   		}
	});
}

function verificaAlteracao() {
	if ( document.getElementById('alteracaodados').value == "1" ) {
			return 'Atenção. Existem dados do formulário que não foram guardados.';
	}
}

function situacaoIndicacao(situacao,aliid) {
	window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&aba=orientadorestudo&requisicao=alterarStatusIndicacao&status='+situacao+'&aliid='+aliid;
}

function exibirInformacoes() {
	jQuery("#modalInformacoes").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function exibirInformacoes2() {
	jQuery("#modalInformacoes2").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });
}

function exibirMunicipiosAtuacao(iuscpf) {
	ajaxatualizar('requisicao=exibirMunicipiosAtuacao&iuscpf='+iuscpf,'modalHistoricoUsuario');
	
	jQuery("#modalHistoricoUsuario").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function atualizarMunicipioAtuacao(iusd,muncod) {
	if(muncod=='') {
		alert('Selecione um Município de atuação');
		return false;	
	}
	
	ajaxatualizar('requisicao=atualizarMunicipioAtuacao&iusd='+iusd+'&muncod='+muncod,'');
		
	window.location=window.location;

}


window.onbeforeunload = verificaAlteracao;

function trocarOrientadorEstudoMunicipio(iusd) {
	if(document.getElementById('fioid_'+iusd).value=='') {
		alert('Selecione um usuário. Este usuário deve ter sido cadastrado pelo Coordenador da IES como Ouvinte na aba Formação Inicial');
		return false;
	}

	var conf = confirm('Deseja realmente efetuar a troca do Orientador de Estudo Selecionado?');
	
	if(conf) {
		window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=trocarOrientadorEstudoMunicipio&iusd_antigo='+iusd+'&fioid='+document.getElementById('fioid_'+iusd).value;
	}
}

</script>

<div id="modalInformacoes" style="display:none;" >
<p>O número de professores alfabetizadores de cada rede foi definido de acordo com o número de turmas do ciclo de alfabetização informado no Censo Escolar 2014. Por exemplo: se o município possui 40 turmas do ciclo de alfabetização e 38 professores atuando nessas turmas (considerando que dois respondem por mais de uma turma), foi utilizado como referência o total de 40 turmas. Se, ao contrário, o município possui 42 professores porque algumas turmas possuem mais de um professor em sala de aula, também foi considerado o total de turmas, ou seja, os mesmos 40.</p>
<p>Logo, é possível que haja pequenas divergências, mas dificilmente esta variação afetará o número final de Orientadores de Estudo do seu estado ou município.</p>
</div>
<div id="modalInformacoes2" style="display:none;" >
<p>Os municípios com menos de 10 Professores Alfabetizadores tem duas opções:</p>
<p>
1) indicar o seu próprio Orientador de Estudos;<br>
2) incluir seus professores nas turmas da rede estadual.
</p>
<p>No segundo caso, o município solicita à rede estadual a inclusão dos seus professores, mas dependerá da aprovação ou reprovação da solicitação.</p>
<p>Em caso afirmativo, a Rede Municipal não poderá selecionar um Orientador de Estudos nem o Coordenador Local poderá receber a bolsa de estudo do Pacto. E, também neste caso, a responsabilidade pelo registro de frequência e liberação das bolsas dos professores alfabetizadores será da Rede Estadual.</p>
</div>

<div id="modalImportarOrientadoresSispactoAnterior" style="display:none;" ></div>

<div id="modalHistoricoUsuario" style="display:none;"></div>


<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" id="requisicao" value="inserirOrientadoresEstudo">
<input type="hidden" name="goto" id="goto" value="">
<input type="hidden" name="alteracaodados" id="alteracaodados" value="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Orientadores de Estudo</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Orientações</td>
	<td>
	<? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Número de Professores Alfabetizadores da Rede (conforme número de turmas indicadas do Censo Escolar 2014)</td>
	<td><?=campo_texto('totalalfabetizadores', "N", "N", "Número de professores Alfabetizadores da Rede (CENSO-2014)", 8, 7, "#######", "", '', '', 0, 'id="totalalfabetizadores"', '', $totalalfabetizadores['total']); ?> <span style="cursor:pointer" onclick="exibirInformacoes();"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda este número</span></td>
</tr>
<? if($_SESSION['sispacto3']['esfera']=='estadual') : ?>
<tr>
	<td class="SubTituloDireita" width="25%">Municípios que desejam incluir professores na sua rede</td>
	<td>
	<?
	$sql = "SELECT m.mundescricao, 
			CASE WHEN alistatus='A' THEN 'Ativo' WHEN alistatus='P' THEN 'Pendente' WHEN alistatus='I' THEN 'Inativo' END as alistatus, 
			a.aliquantidade, 
			CASE WHEN alistatus='P' THEN '<center>".((!$consulta || $db->testa_superuser())?"<input type=button value=Aprovar onclick=\"situacaoIndicacao(\'A\', '||a.aliid||');\"> <input type=button name=alfabetizadoresindicados value=Reprovar onclick=\"situacaoIndicacao(\'I\', '||a.aliid||');\">":"")."</center>'
				 WHEN alistatus='I' THEN '<center>".((!$consulta || $db->testa_superuser())?"<input type=button value=Aprovar onclick=\"situacaoIndicacao(\'A\', '||a.aliid||');\">":"")."</center>' 
				 WHEN alistatus='A' THEN '<center>".((!$consulta || $db->testa_superuser())?"<input type=button value=Reprovar onclick=\"situacaoIndicacao(\'I\', '||a.aliid||');\">":"")."</center>'
				 END as acao,
			e.esddsc
			FROM sispacto3.alfabetizadoresindicados a
	        INNER JOIN territorios.municipio m ON m.muncod = a.alimuncodorigem 
	        INNER JOIN sispacto3.pactoidadecerta p ON p.muncod = a.alimuncodorigem 
	        INNER JOIN workflow.documento d ON d.docid = p.docid 
	        INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			WHERE a.aliestufdestino='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf']."'";
	
	$cabecalho = array("Município solicitante","Situação","Quantidade de professores","&nbsp;","&nbsp;");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="25%">Número de orientadores de estudos a serem cadastrados</td>
	<td><?=campo_texto('total_a_serem_cadastrados', "N", "N", "Número de orientadores de estudos a serem cadastrados", 8, 7, "#######", "", '', '', 0, 'id="total_a_serem_cadastrados"', '', $totalalfabetizadores['total_orientadores_a_serem_cadastrados']); ?></td>
</tr>
<?
$pactoidadecerta = $db->pegaLinha("SELECT picselecaopublica, picincluirprofessorrede FROM sispacto3.pactoidadecerta WHERE picid='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid']."'");
$picselecaopublica = $pactoidadecerta['picselecaopublica'];
$picincluirprofessorrede = $pactoidadecerta['picincluirprofessorrede'];

if($totalalfabetizadores['total'] <= 10) :

	$alfabetizadoresindicados = $db->pegaLinha("SELECT aliid, aliestufdestino, alistatus FROM sispacto3.alfabetizadoresindicados WHERE alimuncodorigem='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod']."'");

?>
<tr>
	<td class="SubTituloDireita" width="25%">Deseja incluir professores em outra rede?</td>
	<td><input type="radio" <?=(($alfabetizadoresindicados['alistatus'] == 'A')?"disabled":"") ?> id="picincluirprofessorrede_TRUE" name="picincluirprofessorrede" <?=(($picincluirprofessorrede=="t")?"checked":"") ?> value="TRUE" onclick="if(this.checked){desejaIncluirProfessoresFormacao(this);}" <?=(($consulta)?"disabled":"") ?> > Sim <input type="radio" name="picincluirprofessorrede"  <?=(($alfabetizadoresindicados['alistatus'] == 'A')?"disabled":"") ?> id="picincluirprofessorrede_FALSE" value="FALSE" onclick="if(this.checked){desejaIncluirProfessoresFormacao(this);}" <?=(($picincluirprofessorrede=="f")?"checked":"") ?> <?=(($consulta)?"disabled":"") ?>> Não   <span style="cursor:pointer" onclick="exibirInformacoes2();"><img src="../imagens/ajuda.png" width="10" height="10"> Entenda esta opção</td>
</tr>
<tr id="tr_mm" <?=(($picincluirprofessorrede=="t")?'':'style="display:none;"') ?>>
	<td class="SubTituloDireita" width="25%">Confirma o estado para incluir os professores</td>
	<td>
	<?
	$sql = "SELECT estuf as codigo, estuf||' - '||estdescricao as descricao FROM territorios.estado WHERE estuf='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf']."'";
	$arrEst = $db->pegaLinha($sql);
	echo campo_texto('descricao', "N", "N", "Estado de destino", 67, 150, "", "", '', '', 0, 'id="descricao"', '', $arrEst['descricao'] );
	echo "<input type=\"hidden\" name=\"aliestufdestino\" value=\"".$arrEst['codigo']."\">";

	if($alfabetizadoresindicados['alistatus'] == 'P') {
		echo " <font size=1>* Aguardando aprovação do estado</font>";
	}
	if($alfabetizadoresindicados['alistatus'] == 'I') {
		echo " <font size=1>* Estado não aprovou a inclusão dos professores</font>";
	}
	if($alfabetizadoresindicados['alistatus'] == 'A') {
		echo " <font size=1>* Estado aprovou a inclusão dos professores</font>";
	}
	?>
	</td>
</tr>
<?
endif;
?>
<tr id="tr_orientadoresestudo">
	<td colspan="2">
	<? if(!$consulta) : ?>
	<p align="right"><input type="button" name="inserirorientadorestudo" value="Inserir Orientador de Estudo" onclick="inserirOrientadorEstudo();"> <input type="button" name="importarsispacto2014" value="Importar SISPACTO 2014" onclick="importarOrientadoresSispactoAnterior();"></p>
	<br/>
	<? endif; ?>
	
	<table class="listagem" id="tabelaorientadoresestudo" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" width="5%">&nbsp;</td>
		<td class="SubTituloCentro" width="10%">CPF</td>
		<td class="SubTituloCentro" width="15%">Nome</td>
		<td class="SubTituloCentro" width="15%">E-mail</td>
		<td class="SubTituloCentro" width="10%">UF/Município</td>
		<td class="SubTituloCentro" width="25%">Tipo</td>
		<td class="SubTituloCentro" width="20%">Documento comprobatório</td>		
	</tr>
	<? if($orientadoresestudo) : ?>
	<? foreach($orientadoresestudo as $oe) : ?>
	<tr>
		<td width="5%">
		<? $aedid_ant = $db->pegaUm("SELECT h.aedid FROM workflow.documento d inner join workflow.historicodocumento h ON h.hstid = d.hstid WHERE d.docid='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['docid']."'"); ?>
		<? if(!$consulta || $db->testa_superuser()) : ?>
		<center><img src="../imagens/excluir.gif" border="0" style="cursor:pointer;" id="img_<?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?>_<?=$oe['muncodatuacao'] ?>" onclick="removerOrientadorEstudo(<?=$oe['iusd'] ?>, this);"></center>
		<? endif; ?>
		
		</td>
		<td width="10%"><?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?></td>
		<td width="15%">
		<? 
		unset($combo_troca);
		if($oe['uncid']) $esdidformacaoinicial = $db->pegaUm("SELECT d.esdid FROM sispacto3.universidadecadastro u INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial WHERE u.uncid='".$oe['uncid']."'");
		
		if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_LOCAL && $esdidformacaoinicial == ESD_FECHADO_FORMACAOINICIAL) {
			if($oe['iusformacaoinicialorientador']=='t') {
				echo "<img src=\"../imagens/check_checklist.png\" width=\"15\" height=\"15\" onmouseover=\"return escape('Orientador de Estudo PRESENTE no curso de Formação Inicial');\"> ";
			}
			
			if($oe['iusformacaoinicialorientador']=='f') {
				echo "<img src=\"../imagens/exclamacao_checklist.png\" width=\"15\" height=\"15\" onmouseover=\"return escape('Orientador de Estudo AUSENTE no curso de Formação Inicial');\"> ";
			}
			
			if(substr($oe['iuscpf'],0,3)=='SIS') {
				echo "<img src=\"../imagens/erro_checklist.png\" align=\"absmiddle\" width=\"15\" height=\"15\"  onmouseover=\"return escape('Orientado de estudo CRIADO PELO SISTEMA deve ser substituído por Orientador de Estudo PRESENTE na Formação Inicial');\"> ";
			}
				
			$wh[] = "fiostatus='A'";
			if($_SESSION['sispacto3']['esfera']=='municipal') $wh[] = "fioesfera='M' AND muncod='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['muncod']."'";
			elseif($_SESSION['sispacto3']['esfera']=='estadual') $wh[] = "fioesfera='E' AND estuf='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['estuf']."'";
			
			$sql = "SELECT fioid FROM sispacto3.formacaoinicialouvintes WHERE iusd='".$oe['iusd']."'";
			$fioid_p = $db->pegaUm($sql);
			
			$sql = "SELECT fioid as codigo, fionome as descricao FROM sispacto3.formacaoinicialouvintes 
					WHERE ".(($wh)?implode(" AND ",$wh):"")." ORDER BY fionome";
			
			$ddcombo = $db->carregar($sql);
			
			if($ddcombo[0]) $combo_troca = $db->monta_combo('fioid', $ddcombo, 'S', 'Selecione', '', '', '', '', 'N', 'fioid_'.$oe['iusd'], true, $fioid_p);
			
		}
		 
		echo $oe['iusnome']; 
		
		if($combo_troca) {
			echo "<br>".$combo_troca." <input type=button value=Atualizar onclick=\"trocarOrientadorEstudoMunicipio('".$oe['iusd']."');\">";
		}
		
		?>
		
		</td>
		<td width="15%" id="td_img_<?=mascaraglobal($oe['iuscpf'],"###.###.###-##") ?>_<?=$oe['muncodatuacao'] ?>"><?=$oe['iusemailprincipal'] ?></td>
		<td width="10%"><?=$oe['municipiodescricaoatuacao'] ?> <?=(($_SESSION['sispacto3']['esfera']=='estadual')?"<img src=../imagens/arrow_v.png style=cursor:pointer; align=absmiddle onclick=\"exibirMunicipiosAtuacao('".$oe['iuscpf']."');\">":"")  ?></td>
		<td width="25%"><?=$_TIPO_ORIENTADORES[$oe['iustipoorientador']] ?></td>
		<td width="20%"><?
		if($oe['iustipoorientador']=="profissionaismagisterio" && substr($oe['iuscpf'],0,3)!='SIS') {
			
			$sql = "SELECT arqnome||'.'||arqextensao as arquivo, a.arqid, p.ponid
					FROM public.arquivo a 
					INNER JOIN sispacto3.portarianomeacao p ON a.arqid = p.arqid 
					WHERE iusd='".$oe['iusd']."'";
			
			$arquivo = $db->pegaLinha($sql);
			
			if($arquivo) {
				echo "<img src=\"../imagens/anexo.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"window.location='sispacto3.php?modulo=principal/coordenadorlocal/coordenadorlocal&acao=A&requisicao=downloadDocumento&arqid=".$arquivo['arqid']."';\">".(($consulta || $aedid_ant==AED_AUTORIZAR_TROCA_ORIENTADORES)?"":" <img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"removerAnexoPortaria('".$arquivo['ponid']."')\">")." ".$arquivo['arquivo'];
			} else {
				echo "<input type=\"file\" name=\"anexoportaria[".mascaraglobal($oe['iuscpf'],"###.###.###-##")."]\">";
			}
			
		} else {
			echo "&nbsp;";
		}
		?></td>
	</tr>
	
	<? endforeach; ?>
	<? endif; ?>
	<tr>
		<td align="center"><img src="../imagens/seta_filho.gif" id="img_setafilho_<?=$oe['iusd'] ?>" title="<?=(($resp['cadastrojustificativa'])?'location':'') ?>"></td>
		<td colspan="6"><b>Total de Orientadores de Estudo : <?=count($orientadoresestudo) ?></b></td>
	</tr>
	
	</table>
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">&nbsp;</td>
	<td>
	<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'],'funcao' => 'salvarOrientadoresEstudos')) ?>
	</td>
</tr>
</table>
</form>