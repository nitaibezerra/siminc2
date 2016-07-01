<?php

if( $_REQUEST["requisicao"] == "enviaescolas" ){
		
	if(is_array($_REQUEST['escolas'])){
		
		$where = '';
		if($_SESSION['estuf'] && checkPerfil(PERFIL_ANALISTA_SEDUC)){
			$where = " AND mun.estuf = '{$_SESSION['estuf']}' ";
		}
		
		$sql = '';
		foreach($_REQUEST['escolas'] as $escola){
			
			if($escola){
			
				$sqlEmi = "select * from em.emiensinomedioinovador where entid = '{$escola}'";
				$rsEntidade = $db->pegaLinha($sqlEmi);
				
				if(!$rsEntidade){
											
					$sqlEnt = "select entcodent from entidade.entidade where entid = ".$escola;
					$entcodent = $db->pegaUm($sqlEnt);
					
					$sql .= "insert into em.emiensinomedioinovador (entid, emianoreferencia, entcodent, emistatus, emipagofnde)
							 values ({$escola}, '".date('Y')."', '{$entcodent}', 'A', 'f'); ";
						
				}elseif($rsEntidade['emistatus'] != 'A'){
					
					$sql .= " update em.emiensinomedioinovador set emistatus = 'A' where entid = '{$escola}'; ";					
				}
			}
		}
				
		if(is_array($_REQUEST['escolas'])){
			if(count($_REQUEST['escolas']) > 0 && !empty($_REQUEST['escolas'])){
				$sql .= " update 
								em.emiensinomedioinovador 
						   set 
								emistatus = 'I' 
						   where 
						   		emiid in (
						   			select 
						   				emiid 
						   			from 
						   				em.emiensinomedioinovador emi
									inner join 
										entidade.endereco ede on ede.entid = emi.entid
									inner join 
										territorios.municipio mun on mun.muncod = ede.muncod 
									where
										emi.entid not in (".implode(', ', $_REQUEST['escolas']).")
									{$where}												
								); ";
			}
		}
		
		$db->executar($sql);
		if($db->commit()){
			$db->sucesso('principal/vincularEscola');
			
		}
		die("<script>
				alert('Erro ao salvar escola!');
				document.location.href = 'em.php?modulo=principal/vincularEscola&acao=A';
				</script>");
	}		
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo("Cadastro de Escolas", "");

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
jQuery.noConflict();

jQuery(function(){	
	jQuery('#escolas').next().next().attr('size','8');	
});

function enviaEscolas()
{
	selectAllOptions( document.getElementById( 'escolas' ) );
	document.formulario.submit();
}
</script>
<form method="post" id="formulario" name="formulario" action="">
	<input type="hidden" name="requisicao" value="enviaescolas"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td width="190px;" class="subtitulodireita">Escolas</td>
			<td>
				<?php
				
					$stWhere = '';
					if($_SESSION['estuf'] && checkPerfil(PERFIL_ANALISTA_SEDUC)){
						$stWhere .= " AND tm.estuf = '{$_SESSION['estuf']}' ";
					}
				
					$sql = "SELECT
								e.entid as codigo, 
								e.entcodent || ' - ' || entnome || ' ( ' || tm.mundescricao || ' )' as descricao
							FROM 
								em.emiensinomedioinovador em 
							LEFT JOIN 
								entidade.entidade e ON e.entid = em.entid
							INNER JOIN
								entidade.endereco eed ON eed.entid = e.entid
							LEFT JOIN
								territorios.municipio tm ON tm.muncod = eed.muncod 
							WHERE 
								emianoreferencia = '{$_SESSION['exercicio']}' AND emistatus='A'
							{$stWhere}";
					
					$escolas = $db->carregar($sql);
					
					$sql = "SELECT DISTINCT
								ee.entid as codigo, 
								ee.entcodent || ' - ' || replace(entnome,'\'','') || ' ( ' || replace(tm.mundescricao,'\'',' ') || ' )' as descricao 
							FROM 
								entidade.entidade ee
							--INNER JOIN
								--educacenso_2011.tab_matricula censo on ee.entcodent::varchar = censo.fk_cod_entidade::varchar 
									--AND censo.fk_cod_etapa_ensino in (25,26,27,28,29,30,31,32,33,34,35,36,37,38)
                               		--AND censo.id_status = 1
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = ee.entid AND funid = 3
							INNER JOIN
								entidade.entidadedetalhe ed ON ed.entcodent = ee.entcodent
							INNER JOIN
								entidade.endereco eed ON eed.entid = ee.entid
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
							and 
								ee.entcodent not ilike 'EN%'
							and 
								length(ee.entcodent) = 8
							{$stWhere}
							ORDER BY
								descricao";
	
					$where = array( 
									array("codigo" 	  => "ee.entcodent",
 								 		  "descricao" => "Código INEP",
 										  "tipo" 	  => 0),
									array("codigo" 	  => "tm.estuf",
 								 		  "descricao" => "UF",
 										  "tipo" 	  => 0),
									array("codigo" 	  => "ee.entnome",
 								 		  "descricao" => "Nome da Escola",
 										  "tipo" 	  => 1)
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
				<input type="button" value="Salvar" onclick="enviaEscolas();" style="cursor: pointer;" />
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>