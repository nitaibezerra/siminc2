<?php

if( $_REQUEST["requisicao"] == "enviaescolas" ){
	$emi->insereEscolas( $_REQUEST );
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo("Cadastro de Escolas", "");

if( $_SESSION["emi"]["emeidPai"] ){
	$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"] );
}

?>
<form method="post" id="emiSelecionaEscolas" action="">
	<input type="hidden" name="requisicao" value="enviaescolas"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td width="190px;" class="subtitulodireita">Escolas</td>
			<td>
				<?php
				
					$sql = "SELECT
								e.entid as codigo, 
								e.entcodent || ' - ' || entnome || ' ( ' || tm.mundescricao || ' )' as descricao
							FROM 
								emi.ementidade em 
							LEFT JOIN 
								entidade.entidade e ON e.entid = em.entid
							INNER JOIN
								entidade.endereco eed ON eed.entid = e.entid
							LEFT JOIN
								territorios.municipio tm ON tm.muncod = eed.muncod 
							WHERE 
								tppid='".EMI_TIPO_ENTIDADE_ESCOLA."' AND emiexercicio = '{$_SESSION['exercicio']}' AND emeidpai='".$_SESSION["emi"]["emeidPai"]."' AND emestatus='A'";
					$escolas = $db->carregar($sql);
					
					if(!$escolas){
						$sql = "SELECT
								e.entid as codigo, 
								e.entcodent || ' - ' || entnome || ' ( ' || tm.mundescricao || ' )' as descricao
							FROM 
								emi.ementidade em 
							LEFT JOIN 
								entidade.entidade e ON e.entid = em.entid
							INNER JOIN
								entidade.endereco eed ON eed.entid = e.entid
							LEFT JOIN
								territorios.municipio tm ON tm.muncod = eed.muncod 
							WHERE 
								tppid='".EMI_TIPO_ENTIDADE_ESCOLA."' AND emiexercicio = '".((int)$_SESSION['exercicio'] - 1 )."' AND emeidpai='".$_SESSION["emi"]["emeidPai"]."' AND emestatus='A'";

					$escolas = $db->carregar($sql);
					}
				
				
					$emeqtdescolas = $db->pegaUm("SELECT emeqtdescolas FROM emi.ementidade WHERE emiexercicio = '{$_SESSION['exercicio']}' AND emeid = {$_SESSION["emi"]["emeidPai"]}");
				
					$sql = "SELECT DISTINCT
								ee.entid as codigo, 
								ee.entcodent || ' - ' || entnome || ' ( ' || replace(tm.mundescricao,'\'',' ') || ' )' as descricao 
							FROM 
								entidade.entidade ee
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = ee.entid AND funid = 3
							INNER JOIN
								entidade.entidadedetalhe ed ON ed.entcodent = ee.entcodent
							INNER JOIN
								entidade.endereco eed ON eed.entid = ee.entid AND estuf = UPPER('".strtoupper($_SESSION["emi"]["estuf"])."')
							LEFT JOIN
								territorios.municipio tm ON tm.muncod = eed.muncod
							WHERE
								(entdreg_medio_medio = '1' OR
								entdreg_medio_integrado = '1'  OR	
								entdreg_medio_normal = '1' OR
								entdreg_medio_prof = '1'  OR
								entdesp_medio_medio = '1'  OR
								entdesp_medio_integrado = '1' OR
								entdesp_medio_normal = '1'  OR
								entdeja_medio = '1' ) AND
								tpcid = 1
							ORDER BY
								descricao";

	
					$where = array( array("codigo" 	  => "ee.entcodent",
 								 		  "descricao" => "Código da Escola",
 										  "tipo" 	  => 0)
 						 );
					combo_popup( 'escolas', $sql, '', '400x600', 0, array(), '', 'S', false, false, null, 500, null, null, null, $where);
				?>
				<br>
				<input type="text" class="disabled" size="3" style="color:red;" value="<?=count($escolas) ?>"/>
				Itens Selecionados
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td></td>
			<td>
				<input type="button" value="Salvar" onclick="enviaEscolas();" style="cursor: pointer;" <?=$emiDisabled; ?>/>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>