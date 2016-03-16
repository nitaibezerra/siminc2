<?php

if($_REQUEST["entid"] && $_REQUEST["memid"]){
	$_SESSION["meentid"] = $_REQUEST["entid"];
	$_SESSION["memid"] = $_REQUEST["memid"];
	
	echo '<script>
			location.href="pdeescola.php?modulo=meprincipal/dados_escola&acao=A";
		  </script>
		 '; 
	exit;
}

/*
if($_POST['ajaxestuf']) {
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where estuf = '".$_POST['ajaxestuf']."'
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione um Município', '', '', '', 215, 'N', '', ''));
}


//MONTA A CLAUSULA WHERE ESTADO/MUNICIPIO
$andUfMun = "";
if($_POST['estuf']){
	$andUfMun .= " and ed.estuf = '{$_POST['estuf']}' ";
}
if($_POST['muncod']){
	$andUfMun .= " and mu.muncod = '{$_POST['muncod']}' ";
}




ini_set("memory_limit", "5120M");

//finaliza escolas
if($_REQUEST["varaux"]){
	
	include_once APPRAIZ . 'includes/workflow.php';
	
	//finaliza escolas rurais 2012 validadas
	if($_REQUEST["varaux"] == '1'){
	
		$sql = "select me.memid, me.docid
					from pdeescola.memaiseducacao me
					inner join entidade.entidade et using(entid)
					inner join entidade.endereco ed using(entid)
					inner join territorios.municipio mu using(muncod)
					inner join workflow.documento d using (docid)
					inner join workflow.estadodocumento esd using (esdid)
					where me.memstatus = 'A'
					and me.memclassificacaoescola = 'R'
					and me.memanoreferencia = 2012
					and esd.esdid = 33
					$andUfMun
					and me.memid not in (
						--ESCOLAS COM TOTAL DE ATIVIDADES DIFERENTE DE 4 
						select memid 
						from pdeescola.meatividade me
						inner join pdeescola.metipoatividade t using(mtaid)
						inner join pdeescola.memaiseducacao e using(memid)
						left join entidade.entidade et using(entid)
						left join entidade.endereco ed using(entid)
						left join territorios.municipio mu using(muncod)
						where e.memstatus = 'A'
						and e.memclassificacaoescola = 'R'
						and e.memanoreferencia = 2012
						
						group by me.memid
						having count(me.meaid) <> 4
					
					UNION
						--ESCOLAS COM PST
						select memid 
						from pdeescola.meatividade me
						inner join pdeescola.metipoatividade t using(mtaid)
						inner join pdeescola.memaiseducacao e using(memid)
						left join entidade.entidade et using(entid)
						left join entidade.endereco ed using(entid)
						left join territorios.municipio mu using(muncod)
						where e.memstatus = 'A'
						and e.memclassificacaoescola = 'R'
						and e.memanoreferencia = 2012
						
						and t.mtaid in (730) --COM PST
						group by me.memid
					
					UNION
						--ESCOLAS SEM CAMPO DO CONHECIMENTO
						select me.memid
						from pdeescola.memaiseducacao me
						inner join entidade.entidade et using(entid)
						inner join entidade.endereco ed using(entid)
						inner join territorios.municipio mu using(muncod)
						where me.memstatus = 'A'
						and me.memclassificacaoescola = 'R'
						and me.memanoreferencia = 2012
						
						and me.memid not in 
							(
							select distinct a.memid
							from pdeescola.meatividade a 
							inner join pdeescola.metipoatividade b 
								using(mtaid)
							where a.meaano = 2012
							and mtaid = 676 --CAMPOS DO CONHECIMENTO
							order by 1
							)
						and me.memid not in 
							(
							select distinct a.memid
							from pdeescola.meatividade a 
							inner join pdeescola.metipoatividade b 
								using(mtaid)
							where a.meaano = 2011
							and mtaid = 676 --CAMPOS DO CONHECIMENTO
							order by 1
							)
					
					UNION
						--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
						select e.memid
						from pdeescola.memaiseducacao e
						left join entidade.entidade et using(entid)
						left join entidade.endereco ed using(entid)
						left join territorios.municipio mu using(muncod)
						left join ( 
							select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
							  from pdeescola.mealunoparticipante 
							 where mapano = 2012 group by memid ) p ON p.memid = e.memid
						left join ( 
							select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
							  from pdeescola.mecenso 
							 where mecanoreferencia = 2012 group by entcodent ) m on e.entcodent = m.entcodent
						where e.memstatus = 'A'
						
						and e.memclassificacaoescola = 'R'
						and e.memanoreferencia = 2012
						and p.mapquantidade > m.mecquantidadealunos
					
					UNION
						--ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
						select distinct a.memid
						from pdeescola.meatividade a 
						inner join pdeescola.mealunoparticipanteatividade b using(meaid)
						inner join pdeescola.mealunoparticipante	  p 
							on a.memid = p.memid
						inner join pdeescola.memaiseducacao e
							on e.memid = a.memid
						where a.meaano = 2012
						and   p.mapano = 2012
						and e.memanoreferencia = 2012
						and e.memstatus = 'A'
						and e.memclassificacaoescola = 'R'
						
						group by a.memid, p.memid, a.mtaid
						having sum(b.mpaquantidade) <> sum(mapquantidade)
					UNION
						--ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES
						Select memid from
			            (
			            
			                        select  mme.memid, eaba.eatid
			                          from pdeescola.memaiseducacao mme
			                          left join pdeescola.meeabatividade eaba 
			                                   using (memid)
			                          left join pdeescola.eabtipoatividade eat 
			                                   using (eatid)
			                          left join pdeescola.eabtipooficina eao 
			                                   using (eaoid)
			                          left join pdeescola.eabtipoduracaooficina ead 
			                                    using (eadid)
			                          left join pdeescola.eabdiarealizacao edr 
			                                     on edr.edrid = eaba.edrid
			                          left join pdeescola.eabtipoespaco eae 
			                                   using (eaeid)
			                         where  mme.memstatus = 'A'
			                           and   mme.memclassificacaoescola = 'R'
			                           and   mme.memanoreferencia = 2012
			                           and   mme.mamescolaaberta = 't'
			                        group by mme.memid, eaba.eatid 
			            )
			            as Total
			                        group by memid
			                        having count (memid) < 4
					
					) ";
	}
	
	$dados = $db->carregar($sql);
	
	foreach($dados as $d){

		$docid = (integer) $d['docid'];
		$memid = $d['memid'];
		$aedid = 68;
		$cmddsc = '';
		$dadosVerificacao = array("memid"=>$memid);

		wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
	}
	
	echo '<script>
			alert("Operação efetuada com sucesso!");
			location.href="pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A";
		  </script>
		 '; 
	exit;
	
}
*/

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Validação de Escolas - Mais Educação', '-' );
 
/*
$mostraUf = "display:";
if(!$_REQUEST['tiporel'] || $_REQUEST['tiporel'] == '1') $mostraUf = "display: none;";
*/
?>

<form name="formulario" id="formulario" method="post" action="">
<input type="hidden" name="varaux" value="">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
<tr>
	<td width="42%" class="SubTituloDireita" valign="top">Tipo Relatório:</td>
	<td>
		<?php
		$sql = "SELECT
					'1' AS codigo,
					'Escolas do Campo' AS descricao
				UNION ALL
				SELECT
					'2' AS codigo,
					'Escolas Urbanas 2012' AS descricao									
				UNION ALL
				SELECT 
					'3' AS codigo,
					'Escolas Urbanas 2011' AS descricao";
		$db->monta_combo( "tiporel", $sql, 'S', 'Selecione o Tipo', 'mostraUf', '', '', 215, 'S', '', '', $_REQUEST['tiporel'] );
		?>
	</td>
</tr>
<!-- 
<tr>
	<td class="SubTituloDireita" valign="top">Situação:</td>
	<td>
		<?php
		/*
		$sql = "SELECT
					'1' AS codigo,
					'PENDENTES' AS descricao
					
				UNION ALL
				SELECT 
					'2' AS codigo,
					'VALIDADAS' AS descricao";
		$db->monta_combo( "tiposit", $sql, 'S', 'Selecione o Tipo', '', '', '', 215, 'S', '', '', $_REQUEST['tiposit'] );
		*/
		?>
	</td>
</tr>
 -->
<!-- 
<tr id="trUf" style="<?//=$mostraUf?>">
	<td class="SubTituloDireita" valign="top">Estado</td>
	<td> 
		<?php
		/*
		$sql = "select
				 e.estuf as codigo, e.estdescricao as descricao 
				from
				 territorios.estado e 
				order by
				 e.estdescricao asc";
		$db->monta_combo( "estuf", $sql, 'S', 'Selecione um Estado', 'recuperaMunicipio', '', '', 215, 'S', '', '', $_POST['estuf'] );
		*/
		?>
	</td>
</tr>
<tr id="trMun" style="<?//=$mostraUf?>">
	<td class="SubTituloDireita" valign="top">Município</td>
	<td id="municipio">
		<?
		/*	
			if($_POST['estuf']){
				$sql = "select
					 muncod as codigo, 
					 mundescricao as descricao 
					from
					 territorios.municipio
					where estuf = '{$_POST['estuf']}'
					order by
					 mundescricao asc";
			}
			else{
				$sql = "select
					 muncod as codigo, 
					 mundescricao as descricao 
					from
					 territorios.municipio
					where estuf = 'XX'
					order by
					 mundescricao asc";
			}
			
			$db->monta_combo( "muncod", $sql, 'S', 'Selecione um Município', '', '', '', 215, 'N', '', '', $_POST['muncod'] );
			*/
		?>
	</td>
</tr>
 --> 
 
	<tr>
		<td align="center" colspan="2" bgcolor="#c0c0c0">
			<input type="button" value="Visualizar" onclick="visualizaEscolas()">
			&nbsp;
			<?
			if($_REQUEST['tiporel']){
				echo '<input type="button" name="btnfinaliza" value="Finalizar as escolas validadas (lista abaixo)" onclick="finalizaEscolas()">';
			}
			?>
		</td>
	</tr>
</table>

</form>
<?

$memid = array();

if($_REQUEST['tiporel']){
	
	//escolas rurais 2012
	if($_REQUEST['tiporel'] == 1){
		
		//validadas
		//--NOT IN
		$sql = "select '<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'||et.entcodent||'</a>' as acao,
						me.memid,
						d.docid,						 
						et.entnome, 
						mu.estuf, 
						mu.mundescricao, 
						case
							when et.tpcid = 1 then 'Estadual'
							when et.tpcid = 2 then 'Federal'
							when et.tpcid = 3 then 'Municipal'
						end AS Esfera
				from pdeescola.memaiseducacao me
				inner join entidade.entidade et using(entid)
				inner join entidade.endereco ed using(entid)
				inner join territorios.municipio mu using(muncod)
				inner join workflow.documento d using (docid)
				where me.memstatus = 'A'
				and me.memclassificacaoescola = 'R'
				and me.memanoreferencia = 2012
				and d.esdid = 33
				$andUfMun
				and me.memid not in (
					--ESCOLAS COM TOTAL DE ATIVIDADES DIFERENTE DE 4 
					select memid 
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'R'
					and e.memanoreferencia = 2012
					
					group by me.memid
					having count(me.meaid) <> 4
				
				UNION
					--ESCOLAS COM PST
					select memid 
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'R'
					and e.memanoreferencia = 2012
					
					and t.mtaid in (730) --COM PST
					group by me.memid
				
				UNION
					--ESCOLAS SEM CAMPO DO CONHECIMENTO
					select me.memid
					from pdeescola.memaiseducacao me
					inner join entidade.entidade et using(entid)
					inner join entidade.endereco ed using(entid)
					inner join territorios.municipio mu using(muncod)
					where me.memstatus = 'A'
					and me.memclassificacaoescola = 'R'
					and me.memanoreferencia = 2012
					
					and me.memid not in 
						(
						select distinct a.memid
						from pdeescola.meatividade a 
						inner join pdeescola.metipoatividade b 
							using(mtaid)
						where a.meaano = 2012
						and mtaid = 676 --CAMPOS DO CONHECIMENTO
						order by 1
						)
					and me.memid not in 
						(
						select distinct a.memid
						from pdeescola.meatividade a 
						inner join pdeescola.metipoatividade b 
							using(mtaid)
						where a.meaano = 2011
						and mtaid = 676 --CAMPOS DO CONHECIMENTO
						order by 1
						)
				
				UNION
					--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
					select e.memid
					from pdeescola.memaiseducacao e
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					left join ( 
						select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
						  from pdeescola.mealunoparticipante 
						 where mapano = 2012 group by memid ) p ON p.memid = e.memid
					left join ( 
						select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
						  from pdeescola.mecenso 
						 where mecanoreferencia = 2012 group by entcodent ) m on e.entcodent = m.entcodent
					where e.memstatus = 'A'
					
					and e.memclassificacaoescola = 'R'
					and e.memanoreferencia = 2012
					and p.mapquantidade > m.mecquantidadealunos
				
				UNION
					--ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
					select distinct a.memid
					from pdeescola.meatividade a 
					inner join pdeescola.mealunoparticipanteatividade b using(meaid)
					inner join pdeescola.mealunoparticipante	  p 
						on a.memid = p.memid
					inner join pdeescola.memaiseducacao e
						on e.memid = a.memid
					where a.meaano = 2012
					and   p.mapano = 2012
					and e.memanoreferencia = 2012
					and e.memstatus = 'A'
					and e.memclassificacaoescola = 'R'
					
					group by a.memid, p.memid, a.mtaid
					having sum(b.mpaquantidade) <> sum(mapquantidade)
				
				)
				order by estuf, mu.mundescricao, entnome";
		
		
	}
	
	
	//escolas urbanas 2012
	if($_REQUEST['tiporel'] == 2){
		
		//validadas
		//--NOT IN
		$sql = "select '<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'||et.entcodent||'</a>' as acao, 
						me.memid,
						d.docid, 
						et.entnome, 
						mu.estuf, 
						mu.mundescricao, 
						case
							when et.tpcid = 1 then 'Estadual'
							when et.tpcid = 2 then 'Federal'
							when et.tpcid = 3 then 'Municipal'
						end AS Esfera
				from pdeescola.memaiseducacao me
				inner join entidade.entidade et using(entid)
				inner join entidade.endereco ed using(entid)
				inner join territorios.municipio mu using(muncod)
				inner join workflow.documento d using (docid)
				where me.memstatus = 'A'
				and me.memclassificacaoescola = 'U'
				and me.memanoreferencia = 2012
				and d.esdid = 33
				$andUfMun
				and me.entcodent not in (
						 select entcodent 
				                   from pdeescola.memaiseducacao 
						  where memanoreferencia = 2011 
						    and memstatus = 'A'
						    and memclassificacaoescola = 'U'
							)
				and me.memid not in (
					--ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6
					select me.memid
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					 left join entidade.entidade et using(entid)
					 left join entidade.endereco ed using(entid)
					 left join territorios.municipio mu using(muncod)
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					and me.meaano = 2012
					
					group by me.memid, e.entcodent
					having count(me.meaid) < 5 or count(me.meaid) > 6
				UNION
					--ESCOLAS COM PST
					select memid 
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					
					and t.mtaid in (730) --COM PST
					group by me.memid
				UNION
					--ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
					select me.memid
					from pdeescola.memaiseducacao me
					inner join entidade.entidade et using(entid)
					inner join entidade.endereco ed using(entid)
					inner join territorios.municipio mu using(muncod)
					where me.memstatus = 'A'
					and me.memclassificacaoescola = 'U'
					and me.memanoreferencia = 2012
					
					and me.memid not in 
					(
					select distinct a.memid
					from pdeescola.meatividade a 
					inner join pdeescola.metipoatividade b 
						using(mtaid)
					inner join pdeescola.metipoatividade 
						using(mtmid)
					where a.meaano = 2012
					and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
					order by 1
					)
					and me.memid not in 
					(
					select distinct a.memid
					from pdeescola.meatividade a 
					inner join pdeescola.metipoatividade b 
						using(mtaid)
					inner join pdeescola.metipoatividade 
						using(mtmid)
					where a.meaano = 2011
					and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
					order by 1
					)
				UNION
					---ESCOLAS COM MAIS DE 4 MACROCAMPOS
					select memid
					from (
						select mem.memid, mtp.mtmid, mem.entcodent
						  from pdeescola.memaiseducacao mem
						 left join entidade.entidade et 
							using(entid)
						 left join entidade.endereco ed 
							using(entid)
						 left join territorios.municipio mu 
							using(muncod)
						inner join pdeescola.meatividade mea
							using(memid)
						inner join pdeescola.metipoatividade met
							using(mtaid)
						inner join pdeescola.metipomacrocampo mtp
							using(mtmid)
						where memanoreferencia = 2012
						  and memstatus = 'A'
						  and memclassificacaoescola = 'U'
						  
						group by 1,2, 3
						order by 1
					     ) Macro
					group by Macro.memid
					having count(memid) > 4
				UNION
					--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
					select e.memid
					  from pdeescola.memaiseducacao e
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					left join ( select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
						      from pdeescola.mealunoparticipante 
				                     where mapano = 2012 
				                  group by memid ) p on p.memid = e.memid
					left join ( select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
						      from pdeescola.mecenso 
						     where mecanoreferencia = 2012 
						group by entcodent ) m on e.entcodent = m.entcodent
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					
					and p.mapquantidade > m.mecquantidadealunos
				
				UNION
					--NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE
					select memid from (
						select a.memid, qtd1, qtd2
						  from pdeescola.memaiseducacao m
						left join entidade.entidade et using(entid)
						left join entidade.endereco ed using(entid)
						left join territorios.municipio mu using(muncod)
				
						inner join ( select * from pdeescola.meatividade where meaano = 2012 ) a 
							on m.memid = a.memid
						inner join (select memid, mapano, sum(mapquantidade) as qtd1 from pdeescola.mealunoparticipante group by memid, mapano ) b 
							on b.memid = m.memid and b.mapano = m.memanoreferencia
						inner join (select memid, p.meaid, meaano, sum(mpaquantidade) as qtd2
						from pdeescola.mealunoparticipanteatividade p
						inner join pdeescola.meatividade a 
							on p.meaid = a.meaid
						where a.meaano = 2012 
						group by memid, p.meaid, meaano) c 
							on c.meaano = a.meaano and c.memid = a.memid and c.meaid = a.meaid
						where m.memstatus = 'A'
						and m.memanoreferencia = 2012
						and m.memclassificacaoescola = 'U'
						
						and qtd1 = qtd2
						order by memid
					) as x
					group by memid
					having count(memid) < 5
				
				UNION
					--ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
					select distinct a.memid
					from pdeescola.meatividade a 
					inner join pdeescola.mealunoparticipanteatividade b using(meaid)
					inner join pdeescola.mealunoparticipante	  p 
						on a.memid = p.memid
					inner join pdeescola.memaiseducacao e
						on e.memid = a.memid
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
				
					where a.meaano = 2012
					and   p.mapano = 2012
					and e.memanoreferencia = 2012
					and e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					
					group by a.memid, p.memid, a.mtaid
					having sum(b.mpaquantidade) <> sum(mapquantidade)
				
				)
				order by estuf, mu.mundescricao, entnome";
		
	}
	
	
	//escolas urbanas 2011
	if($_REQUEST['tiporel'] == 3){
		
		//validadas
		//--NOT IN
		$sql = "select '<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'||et.entcodent||'</a>' as acao,
						me.memid,
						d.docid, 
						et.entnome, 
						mu.estuf, 
						mu.mundescricao, 
						case
							when et.tpcid = 1 then 'Estadual'
							when et.tpcid = 2 then 'Federal'
							when et.tpcid = 3 then 'Municipal'
						end AS Esfera
				from pdeescola.memaiseducacao me
				inner join entidade.entidade et using(entid)
				inner join entidade.endereco ed using(entid)
				inner join territorios.municipio mu using(muncod)
				inner join workflow.documento d using (docid)
				where me.memstatus = 'A'
				and me.memclassificacaoescola = 'U'
				and me.memanoreferencia = 2012
				and d.esdid = 33
				$andUfMun
				and me.entcodent in (select entcodent 
						       from pdeescola.memaiseducacao 	
						       where memanoreferencia = 2011 
							 and memstatus = 'A' 
							 and memclassificacaoescola = 'U')
				and me.memid not in (
					--ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6
					select me.memid
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					where e.memstatus = 'A'
						
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					and me.meaano = 2012
					group by me.memid
					having count(me.meaid) < 5 or count(me.meaid) > 6
				
				UNION
				
					--ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
					select me.memid
					from pdeescola.memaiseducacao me
					inner join entidade.entidade et using(entid)
					inner join entidade.endereco ed using(entid)
					inner join territorios.municipio mu using(muncod)
					where me.memstatus = 'A'
					and me.memclassificacaoescola = 'U'
					and me.memanoreferencia = 2012
					
					and me.memid not in 	(
				
						select distinct a.memid
						from pdeescola.meatividade a 
						inner join pdeescola.metipoatividade b 
							using(mtaid)
						inner join pdeescola.metipoatividade 
							using(mtmid)
						where a.meaano = 2012
						and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
								)
					and me.memid not in 	(
						select distinct a.memid
						from pdeescola.meatividade a 
						inner join pdeescola.metipoatividade b 
							using(mtaid)
						inner join pdeescola.metipoatividade 
							using(mtmid)
						where a.meaano = 2011
						and mtmid = 42 --MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO
								)
				
				UNION
					---ESCOLAS COM MAIS DE 4 MACROCAMPOS
					select memid
					from (
						select mem.memid, mtp.mtmid, mem.entcodent
						  from pdeescola.memaiseducacao mem
						  inner join pdeescola.meatividade mea
							using(memid)
						  inner join pdeescola.metipoatividade met
							using(mtaid)
						  inner join pdeescola.metipomacrocampo mtp
							using(mtmid)
						  left join entidade.entidade et 
							using(entid)
						  left join entidade.endereco ed 
							using(entid)
						  left join territorios.municipio mu 
							using(muncod)
						  where memanoreferencia = 2012
						    
						    and memstatus = 'A'
						    and memclassificacaoescola = 'U'
						    group by 1,2, 3
					) Macro
					group by Macro.memid
					having count(memid) > 4
				UNION
					--NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE
					select memid 
					  from (
						select a.memid, qtd1, qtd2
						  from pdeescola.memaiseducacao m
						  left join entidade.entidade et 
							using(entid)
						  left join entidade.endereco ed 
							using(entid)
						  left join territorios.municipio mu 
							using(muncod)
						inner join ( select * from pdeescola.meatividade where meaano = 2012 ) a
							on m.memid = a.memid
						inner join (select memid, mapano, sum(mapquantidade) as qtd1 from pdeescola.mealunoparticipante group by memid, mapano ) b 
							on b.memid = m.memid and b.mapano = m.memanoreferencia
						inner join (select memid, p.meaid, meaano, sum(mpaquantidade) as qtd2
						      from pdeescola.mealunoparticipanteatividade p
						inner join pdeescola.meatividade a 
							on p.meaid = a.meaid
						where a.meaano = 2012 
						group by memid, p.meaid, meaano
						) c on c.meaano = a.meaano 
						and c.memid = a.memid 
						and c.meaid = a.meaid
						where m.memstatus = 'A'
						  
						and m.memanoreferencia = 2012
						and m.memclassificacaoescola = 'U'
						and qtd1 = qtd2
						order by memid
						) as x
					group by memid
					having count(memid) < 5
				
				UNION
					--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
					select e.memid
					from pdeescola.memaiseducacao e
					inner join entidade.entidade et using(entid)
					inner join entidade.endereco ed using(entid)
					inner join territorios.municipio mu using(muncod)
				
					left join ( 
						   select memid, sum(coalesce(mapquantidade,0)) as mapquantidade 
						     from pdeescola.mealunoparticipante where mapano = 2012 
						  group by memid ) p ON p.memid = e.memid
					left join ( select entcodent, sum(coalesce(mecquantidadealunos,0)) as mecquantidadealunos 
						      from pdeescola.mecenso where mecanoreferencia = 2012 
						  group by entcodent ) m on e.entcodent = m.entcodent
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					
					and p.mapquantidade > m.mecquantidadealunos
				
				)
				order by estuf, mu.mundescricao, entnome";
		
	}
	
	
	//dbg($sql,1);
	/*
	$cabecalho = array( "Código INEP", "Escola", "Estado", "Municipio", "Esfera");
	$alinha = array("center","left","center","left");
	$tamanho = array("15%","45%","15%","25%");
					
	$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '',$tamanho,$alinha);
	*/
	
	
	//dbg($sql,1);
	$dados = $db->carregar($sql);
	
	
	//lista escolas
	if(!$_REQUEST["varaux"]){
	
		$cabecalho = array( "Código INEP", "Código Mais Escola", "Código DOCID", "Escola", "Estado", "Municipio", "Esfera");
		//$alinha = array("center","center","left","center","left");
		//$tamanho = array("15%","45%","15%","25%");
		//$db->monta_lista( $sql, $cabecalho, 100, 10, 'N', 'center', '', '',$tamanho,$alinha);
		
		$db->monta_lista_array($dados,$cabecalho,50,10,'N','center');
	
	}
	else{ //finaliza escolas
	
		
		
		if($_REQUEST["varaux"] == '1'){
			
			ini_set("memory_limit", "5120M");
			include_once APPRAIZ . 'includes/workflow.php';
			$i=0;
			
			if($dados){ 
				foreach($dados as $d){
			
					$i++;
					$docid = (integer) $d['docid'];
					$memid = $d['memid'];
					$aedid = 68;
					$cmddsc = '';
					$dadosVerificacao = array("memid"=>$memid);
					
					/*
					echo '<br><br> i = '.$i;
					echo '<br> docid = '.$docid;
					*/
					wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
				}
				
				
				echo '<script>
						alert("Operação efetuada com sucesso!");
						location.href="pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A";
					  </script>
					 '; 
			} else {
				echo '<script>
						alert("Não foi possível realizar a operação!");
						location.href="pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A";
					  </script>
					 '; 				
			}
			exit;
		}
	}
}

?>

<script type="text/javascript" src="/includes/prototype.js"></script>

<script>

function visualizaEscolas(){
	if(document.formulario.tiporel.value == ''){
		alert('Campo Obrigatório: Tipo Relatório.');
		document.formulario.tiporel.focus();
		return false;
	}
	/*
	if(document.formulario.tiposit.value == ''){
		alert('Campo Obrigatório: Situação.');
		document.formulario.tiposit.focus();
		return false;
	}
	/*
	
	/*
	if(document.formulario.tiporel.value != '1'){
		if(document.formulario.estuf.value == ''){
			alert('Campo Obrigatório: Estado.');
			document.formulario.estuf.focus();
			return false;
		}
		
		if(document.formulario.muncod.value == ''){
			alert('Campo Obrigatório: Município.');
			document.formulario.muncod.focus();
			return false;
		}
		
	}
	*/
	
	document.formulario.submit();
	
}

function finalizaEscolas(flag){
	
	if(confirm('Deseja realmente finalizar as escolas da lista abaixo?')){
		document.formulario.btnfinaliza.value = 'Processando! Aguarde...';
		document.formulario.btnfinaliza.disabled = true;
		document.formulario.varaux.value = '1';
		document.formulario.submit();
	}
}

/*
function recuperaMunicipio(estuf) {
	var tdMunicipio	= document.getElementById('municipio');
	
	var req = new Ajax.Request('pdeescola.php?modulo=meprincipal/meValidaEscolas&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {
										tdMunicipio.innerHTML = res.responseText;
							        }
							  });
}

function mostraUf(tipo){

	var trUf	= document.getElementById('trUf');
	var trMunicipio	= document.getElementById('trMun');
	if(tipo == '1'){
		trUf.style.display = 'none';
		trMunicipio.style.display = 'none';
	}
	else{
		trUf.style.display = '';
		trMunicipio.style.display = '';
		
	}
}
*/

</script>