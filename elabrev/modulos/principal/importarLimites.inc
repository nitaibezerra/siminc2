<?php

if( $_POST['requisicao'] == 'carregarMatriz' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT 
			  mtrid as codigo,
			  mtrdsc as descricao
			FROM 
			  elabrev.matriz 
			WHERE 
				gpmid = {$_POST['gpmid']} 
				and mtrano = '".($_SESSION['exercicio'] + 1)."' order by 2";
	
	echo $db->monta_combo( 'mtrid', $sql, 'S', 'Escolha a Grupo', '', '', '', '', 'S', '' );
	exit();
}

if( $_POST['requisicao'] == 'carregardados' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$gpmid = $_POST['gpmid'];
	$mtrid = $_POST['mtrid'];
	
	$arrDados = processar_entrada_limites( $_POST['dados'] );
	if( $arrDados != 'erro' ){
		
		$ppoid = $db->pegaUm("select ppoid from elabrev.propostaorcamento where ppoanoexercicio = '{$_SESSION['exercicio']}' and tppid = 1 and ppostatus = 'A'");
		
		$sql = "SELECT mm.mtmid, mm.mtmvlrlimite 
				FROM elabrev.montantematriz mm 
				WHERE 
					mtrid = $mtrid 
				    and mm.mtmano = '".($_SESSION['exercicio'] + 1)."'
				    and mm.ppoid = $ppoid";		
		$arrMontante = $db->pegaLinha( $sql );
		
		if( $arrMontante['mtmid'] ){
			$sql = "update elabrev.montantematriz set
						mtmvlrlimite = {$arrDados['valortotal']} 
					where mtmid = ".$arrMontante['mtmid'];
			$db->executar( $sql );
		} else {
			$sql = "insert into elabrev.montantematriz(mtrid, ppoid, mtmvlrlimite, mtmformatofracao, mtmano ) 
					values( " . $mtrid . "," . $ppoid . "," . $arrDados['valortotal'] . ",'I','" . ($_SESSION['exercicio'] + 1) . "') returning mtmid";
			$mtmid = $db->pegaUm( $sql );
		}
		
		$mtmid = ($arrMontante['mtmid'] ? $arrMontante['mtmid'] : $mtmid);
		$sql = "delete from elabrev.limitefonte where mtmid = " . $mtmid;
		$db->executar( $sql );
		foreach ($arrDados as $key => $valor) {
			
			if( (string)$key != 'valortotal' ){
				$sql = "insert into elabrev.limitefonte ( foncod, unicod, lmfvalor, unitpocod, mtmid )
						values('{$valor['fon']}', '{$valor['uni']}', '{$valor['val']}', 'U', $mtmid)";
						
				$db->executar( $sql );
			}
		}
		
		if($db->commit()){
			$db->sucesso('principal/importarLimites');
		} else {
			echo "<script>
					alert('Falha na operação');
					window.location.href = window.location;
				</script>";
		}	
		exit();
	}
}

function processar_entrada_limites( $dados_brutos ){
	
	$dados = array();
	$dados_linhas = explode( "\n", $dados_brutos );
	$totalGeral = 0;
	
	foreach ( $dados_linhas as $linha => $dados_linha ){
		
		if ( !$dados_linha ){
			continue;
		}
		$dados_linha = explode( "\t", $dados_linha );
		$uni = $dados_linha[0];
		$fon = $dados_linha[1];
		$val = (integer) rtrim( str_replace( '.', '', $dados_linha[2] ) );
		$val = (integer) rtrim( str_replace( ',', '', $val ) );
		$totalGeral+= $val;
		if ( count( $dados_linha ) != 3 ){
			echo "<script>alert('Linha ". ( $linha + 1 ) ." está mal formada.');</script>";
			return 'erro';
			die();
		}
		
		$novo_item = array(
			'uni' => $uni,
			'fon' => $fon,
			'val' => $val
		);
		array_push( $dados, $novo_item );
	}
	$dados['valortotal'] = $totalGeral;
	
	return $dados;
}

include APPRAIZ."includes/cabecalho.inc"; 
print "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
$menu = array(0 => array("id" => 1, "descricao" => "Demais unidades orçamentárias",   "link" => "/elabrev/elabrev.php?modulo=principal/despesasgerais&acao=A"),
			  1 => array("id" => 2, "descricao" => "Administração direta",			  "link" => "/elabrev/elabrev.php?modulo=principal/despesasgerais_sub&acao=A"),
			  2 => array("id" => 3, "descricao" => "Importar Limites",			  	  "link" => "/elabrev/elabrev.php?modulo=principal/importarLimites&acao=A")
		  	  );
echo montarAbasArray($menu, "/elabrev/elabrev.php?modulo=principal/importarLimites&acao=A");
monta_titulo( $titulo_modulo, '' );
?>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td width="35%" class="SubTituloDireita">Grupo de Coluna:</td>
			<td><? 
				$sql = "SELECT 
							g.gpmid as codigo, 
						    g.gpmdsc as descricao
						FROM 
							elabrev.grupomatriz g 
						    inner join elabrev.matriz 			m  ON  m.gpmid = g.gpmid 
						    inner join elabrev.montantematriz 	mm ON mm.mtrid = m.mtrid 
						WHERE 
							m.mtrano = '".($_SESSION['exercicio'] + 1)."'  
						group by 
							g.gpmid, 
						    g.gpmdsc, 
						    g.gpmordem 
						order by 
							g.gpmdsc";
			$db->monta_combo( 'gpmid', $sql, 'S', 'Escolha a Grupo', 'carregarMatriz', '', '', '620', 'S', 'gpmid' ); ?></td>
		</tr>
		<tr id="tr_matriz" style="display: none">
			<td class="SubTituloDireita">Matriz:</td>
			<td><div id="combo_matriz"></div></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Dados:</td>
			<td>layout:&nbsp;&nbsp;
				cod_unidade&nbsp;&nbsp;|&nbsp;&nbsp;
				cod_fonte&nbsp;&nbsp;|&nbsp;&nbsp;
				valor
				<br/>
				Um registro por linha separados por tabulação<br>
					<? $dados = $_REQUEST['dados']; ?>
					<?= campo_textarea( 'dados', 'S', 'S', 'Dados', 125, 20, null ) ?>
			</td>
		</tr>
		<tr bgcolor="#cccccc">
			<td>&nbsp;</td>
			<td>
				<input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="carregarDadosMatriz();"/>
			</td>
		</tr>
	</table>
</form>
<div id="debug"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
if( document.getElementById('gpmid').value != '' ){
	carregarMatriz( document.getElementById('gpmid').value );
}
function carregarMatriz(gpmid){
	divCarregando();
	
	//var dados = $('#formulario').serialize();

	$.ajax({
   		type: "POST",
   		url: "elabrev.php?modulo=principal/importarLimites&acao=A",
   		data: "requisicao=carregarMatriz&gpmid="+gpmid,
   		async: false,
   		success: function(msg){
   			$('#combo_matriz').html(msg);
   			document.getElementById('tr_matriz').style.display = '';
   		}
	});

	divCarregado();
}

function carregarDadosMatriz(){
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if(erro == 0){
		document.getElementById('requisicao').value = 'carregardados';
		$("#formulario").submit();
	}
}
</script>