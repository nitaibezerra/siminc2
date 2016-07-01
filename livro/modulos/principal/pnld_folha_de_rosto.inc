<?php 
if( $_REQUEST['req'] == 'salvar' ){
	
	if($_REQUEST['lvdid'] == '')         		 { $null = true; };
	if($_REQUEST['lvdtitulo'] == '')   			 { $null = true; };
	if($_REQUEST['lvdcodigo'] == '')       		 { $null = true; };
	if($_REQUEST['lvdtipo'] == '')         		 { $null = true; };
	if($_REQUEST['lvdanoedicao'] == '')    		 { $null = true; };
	if($_REQUEST['lvdnumeropaginas'] == '')		 { $null = true; };
	if($_REQUEST['lvdautor'] == '')				 { $null = true; };
	/*if($_REQUEST['lvdqtddescaracterizado'] == ''){ $null = true; };
	if($_REQUEST['lvdqtdcaracterizado'] == '')	 { $null = true; };
	if($_REQUEST['lvdqtdtotal'] == '')			 { $null = true; };*/
	
	if( !$null ){
		/*
		 * lvdqtddescaracterizado = ".$_REQUEST['lvdqtddescaracterizado'].",
	    	lvdqtdcaracterizado    = ".$_REQUEST['lvdqtdcaracterizado'].",
	    	lvdqtdtotal 		   = ".$_REQUEST['lvdqtdtotal']."
		 */
		$sql = "UPDATE livro.livrodidatico
			    SET 
			    	lvdtitulo 			   = '".$_REQUEST['lvdtitulo']."',
			    	lvdcodigo 			   = '".$_REQUEST['lvdcodigo']."',
			    	lvdtipo   		       = '".$_REQUEST['lvdtipo']."',
			    	lvdanoedicao 		   = '".$_REQUEST['lvdanoedicao']."',
			    	lvdnumeropaginas 	   = ".$_REQUEST['lvdnumeropaginas'].",
			    	lvdautor 			   = '".$_REQUEST['lvdautor']."'
			    WHERE 
			    	lvdid = ".$_REQUEST['lvdid'];
		$db->executar($sql);
		$db->commit();
		echo "<script>alert('Dados alterados com sucesso!');</script>";
	}
}

$lvdid = $_REQUEST['lvdid'];

if( $lvdid == '' ){
	echo "<script>alert('Escolha um livro.');window.history.back(-1);</script>";
	exit;
}
//ver($lvdid,d);
/**
	lvdqtddescaracterizado, 
	lvdqtdcaracterizado, 
	lvdqtdtotal
 */
$sql = "SELECT 
			lvdtitulo, 
			lvdcodigo, 
			lvdtipo, 
			lvdanoedicao,
			lvdnumeropaginas, 
			lvdautor
		FROM 
			livro.livrodidatico
		WHERE
			lvdid = ".$lvdid;
$dados = $db->pegaLinha( $sql );

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo('Livro Didático','Folha de Rosto')

?>
<form action="" method="post" name="form_livro" id="form_livro">
	<input type="hidden" name="lvdid" value="<?=$lvdid; ?>"/>
	<input type="hidden" name="req" id="req" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" style="border-top: 2px solid #404040" cellspacing="1" cellpadding="3">
		<tr style="border-top: 1px">
			<td class="SubTituloDireita" width="20%">Titulo:</td>
			<td style="border-bottom: 1px solid #DCDCDC">
				<?=campo_texto('lvdtitulo','N','S','',50,50,'','','','','','','',$dados['lvdtitulo']);?>
<!--				<img src="../imagens/busca.gif" width="13px"/>-->
			</td>
			<td rowspan="10" width="40%" bgcolor="white" align="center">
				<strong>Cadastro de Livros</strong>
				<br />
				<img src="../imagens/fila_de_livros.gif" width="100px"/>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código:</td>
			<td style="border-bottom: 1px solid #DCDCDC">
				<?=campo_texto('lvdcodigo','N','S','',12,12,'','','','','','','',$dados['lvdcodigo']);?>
<!--				<img src="../imagens/busca.gif" width="13px"/>-->
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo:</td>
			<td style="border-bottom: 1px solid #DCDCDC">
				<?php
				$tipoArray = Array(Array('codigo' => 'L', 'descricao' => 'Livro do Aluno'),Array('codigo' => 'M', 'descricao' => 'Manual do Professor'));
				
				echo $db->monta_combo('lvdtipo',$tipoArray,'S','Tipo','','','',120,'N','','',$dados['lvdtipo']);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ano Edição:</td>
			<td style="border-bottom: 1px solid #DCDCDC">
				<?php
				$sql = "SELECT
							MIN(lvdanoedicao) as minimo,
							MAX(lvdanoedicao) as maximo
						FROM 
							livro.livrodidatico";
				$anos = $db->pegaLinha($sql);
				$anoArray = Array();
				for( $ano = $anos['minimo']; $ano <= $anos['maximo']; $ano++ ){
					array_push($anoArray, Array('codigo' => $ano, 'descricao' => $ano));
				}
//				ver($dados['lvdanoedicao']);
				echo $db->monta_combo('lvdanoedicao',$anoArray,'S','Ano','','','',80,'N','','',$dados['lvdanoedicao']);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Numero de Páginas:</td>
			<td style="border-bottom: 1px solid #DCDCDC"><?=campo_texto('lvdnumeropaginas','N','S','',12,12,'','','','','','','',$dados['lvdnumeropaginas']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Autor:</td>
			<td style="border-bottom: 1px solid #DCDCDC"><?=campo_texto('lvdautor','N','S','',50,50,'','','','','','','',$dados['lvdautor']);?></td>
		</tr>
		<tr>
			<td colspan="2"></td>
		</tr>
		<!-- <tr>
			<td class="SubTituloDireita">Descaracterizado:</td>
			<td style="border-bottom: 1px solid #DCDCDC"><?=campo_texto('lvdqtddescaracterizado','N','S','',5,5,'','','','','','','',$dados['lvdqtddescaracterizado']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Caracterizado:</td>
			<td style="border-bottom: 1px solid #DCDCDC"><?=campo_texto('lvdqtdcaracterizado','N','S','',5,5,'','','','','','','',$dados['lvdqtdcaracterizado']);?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Total:</td>
			<td style="border-bottom: 1px solid #DCDCDC"><?=campo_texto('lvdqtdtotal','N','S','',5,5,'','','','','','','',$dados['lvdqtdtotal']);?></td>
		</tr> -->
		<tr bgcolor="#DCDCDC">
			<td></td>
			<td colspan="2" align="left">
				<input type="button" value="Salvar" onclick="salvarAlteracoes()"/>
			</td>
		</tr>
	</table>
</form>
<script>
function salvarAlteracoes(){
	var req  = document.getElementById('req');
	var form = document.getElementById('form_livro');

	req.value = 'salvar';
	form.submit();
}
</script>