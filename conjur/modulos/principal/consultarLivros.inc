<?php
include_once APPRAIZ ."includes/workflow.php";

$_SESSION['lvaid'] = $_SESSION['lvaid'] ? $_SESSION['lvaid'] : $_REQUEST['lvaid'];
$_SESSION['lveid'] = $_SESSION['lveid'] ? $_SESSION['lveid'] : $_POST['lveid'];

//insere emprestimo
if($_POST){
		
	if($_POST['lveid']){
		
		$sql = "UPDATE conjur.livroemprestimo
			   	SET advid=".$_POST['advid'].",
			   		lvedtprevdevolucao='". formata_data_sql($_POST['lvedtprevdevolucao'])."'
			 	WHERE lveid = ".$_POST['lveid'];	

		$db->executar( $sql );		
		$db->commit();
	
	
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='conjur.php?modulo=principal/consultarLivros&acao=A&lvaid='".$_SESSION['lvaid']."</script>";
		
	}
		
	
}


$sql = "SELECT 
				lvapatrimonio, 
			 	lvatitulo,
			 	lvaautor,
			 	lvaeditora,
			 	lvaedicao,
			 	lvadtinclusao,
			 	lvastatus,
			 	docid
			FROM  
				conjur.livroacervo
			WHERE 
				lvaid = ".$_SESSION['lvaid'];
$dados = $db->carregar($sql);


//insere emprestimo
$sql = "SELECT	max(lveid) as lveid
			 FROM conjur.livroemprestimo
			 WHERE lvaid = ".$_SESSION['lvaid']."
			 and lvedtrealdevolucao is null
			 and lvedtliberacao is null";
$lveid = $db->pegaUm($sql);
if(!$lveid){
	$sql = "INSERT INTO conjur.livroemprestimo (lvaid) VALUES (".$_SESSION['lvaid'].")";
	$db->executar( $sql );		
	$db->commit();
}
//fim

$sql = "SELECT 
				lveid,
				advid, 
			 	lvedtprevdevolucao
			FROM  
				conjur.livroemprestimo
			WHERE 
				lvaid = ".$_SESSION['lvaid']."
			 and lvedtrealdevolucao is null
			 and lvedtliberacao is null";
$dados2 = $db->pegaLinha($sql);
if($dados2) extract($dados2);


include APPRAIZ ."includes/cabecalho.inc";
print '<br>';


	$menu[0] = array("descricao" => "Biblioteca", "link"=> "conjur.php?modulo=principal/listaBiblioteca&acao=A");
	$menu[1] = array("descricao" => "Livro", "link"=> "conjur.php?modulo=principal/consultarLivros&acao=A");
	echo montarAbasArray($menu, "conjur.php?modulo=principal/consultarLivros&acao=A");


monta_titulo( 'Empréstimo de Livros', 'No fluxo abaixo, você poderá reservar, emprestar e devolver o livro escolhido.' );

//recupera docid
if($_SESSION['lvaid']){
	$docid = pegarDocidB( $_SESSION['lvaid'] );
	if($docid){
		$esdid = pegaEstadoWorkFlow($docid);
	}
}
?>
<body>

<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>

<form id="formulario" name="formulario" action="" method="post" onsubmit="return validaForm();" >

<input type="hidden" name="lvaid" value="<? echo $_SESSION['lvaid']; ?>" />
<input type="hidden" name="lveid" value="<? echo $lveid; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="40%" align='right' class="SubTituloDireita"><b>Patrimônio:</b></td>
		<td>
			<? echo $dados[0]['lvapatrimonio'];  ?>
		</td>
		<td rowspan="8" width="10%" align="center">
			<?php 
			// Barra de estado atual e ações e Historico
			if($_SESSION['lvaid']){
				$dados_wf = array('lvaid' => $_SESSION['lvaid']);
		
				echo '<br>';
				wf_desenhaBarraNavegacao( $docid , $dados_wf );
				echo '<br><br>';
			}	
			?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita"><b>Título:</b></td>
		<td colspan="2">
			<? echo $dados[0]['lvatitulo'];  ?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita"><b>Autor:</b></td>
		<td colspan="2">
			<? echo $dados[0]['lvaautor'];  ?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita"><b>Editora:</b></td>
		<td colspan="2">
			<? echo $dados[0]['lvaeditora'];  ?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita"><b>Edição:</b></td>
		<td colspan="2">
			<? echo $dados[0]['lvaedicao'];  ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"><b>Advogado:</b></td>
		<td><?
			$habil = "S";
			if($esdid != 1074) $habil = "N";
			
			if( (!$db->testa_superuser()) && possuiPerfil( Array(PRF_ADVOGADO) ) ){
				$sql = "SELECT	adv.advid
						 FROM entidade.entidade ent
						 inner join conjur.advogados adv on adv.entid = ent.entid
						 WHERE ent.entnumcpfcnpj = '" . $_SESSION['usucpf'] . "'";
				$advid2 = $db->pegaUm($sql);
				$andAdv = " AND adv.advid in ($advid2)";
			}
			$sqlAdvogado = "SELECT adv.advid as codigo, ent.entnome as descricao 
							FROM conjur.advogados adv 
							LEFT JOIN entidade.entidade ent ON ent.entid = adv.entid
							where adv.advstatus = 'A'
							$andAdv 
							ORDER BY ent.entnome";
			$db->monta_combo('advid', $sqlAdvogado, $habil, ($andAdv ? '':'Selecione...'), '', '', '', 150, 'N', 'advid');
			?></td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita"><b>Data Prevista de Devolução:</b></td>
		<td>
			<?
				echo campo_data('lvedtprevdevolucao', 'N', 'S', '', 'S');
			?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%" >&nbsp;</td>
		<td colspan="2">
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		&nbsp;<input type="button" class="botao" name="voltar" value="Voltar" onclick="javascript:location.href = 'conjur.php?modulo=principal/listaBiblioteca&acao=A';">
		<?
		if($esdid == 1076){
			?>
			&nbsp;<input type="button" class="botao" name="imprimir" value="Imprimir Recibo" onclick="recibo(<?=$_SESSION['lvaid']?>);">
			<?
		}
		?>
		</td>
	</tr>
</table>
</form>

</body>

<script type="text/javascript">

function validaForm(){
	
	if(document.formulario.advid.value == ''){
		alert ('O campo Advogado deve ser preenchido.');
		document.formulario.advid.focus();
		return false;
	}
	if(document.formulario.lvedtprevdevolucao.value == ''){
		alert ('O campo Data Prevista de Devolução deve ser preenchido.');
		document.formulario.lvedtprevdevolucao.focus();
		return false;
	}
	
	var dtdevolucao = document.formulario.lvedtprevdevolucao.value.substr(6,4) + document.formulario.lvedtprevdevolucao.value.substr(3,2) + document.formulario.lvedtprevdevolucao.value.substr(0,2);
	var dtatual = "<?=date('Ymd')?>";
	
	if( parseFloat(dtdevolucao) <  parseFloat(dtatual) ){
		alert('Data Prevista de Devolução não pode ser menor que a data de hoje.');
		document.formulario.lvedtprevdevolucao.focus();
		return false;
	}
	
}

function recibo(lvaid) {
		windowOpen('?modulo=principal/reciboLivro&acao=A&lvaid='+lvaid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	}
	
</script>