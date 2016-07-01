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

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Validação de Escolas Pendentes - Mais Educação', '-' );
 
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
		$db->monta_combo( "tiporel", $sql, 'S', 'Selecione', 'filtroPendencia', '', '', 215, 'S', '', '', $_REQUEST['tiporel'] );
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Pendência:</td>
	<td>
		<?php
		$sql = "SELECT
						'' AS codigo,
						'Selecione o Tipo Relatório' AS descricao";
		
		if($_REQUEST['tiporel'] == 1){
			$sql = "SELECT
						'1' AS codigo,
						'ESCOLAS COM TOTAL DE ATIVIDADES DIFERENTE DE 4' AS descricao
					UNION ALL
					SELECT 
						'2' AS codigo,
						'ESCOLAS COM PST' AS descricao
					UNION ALL
					SELECT 
						'3' AS codigo,
						'ESCOLAS SEM CAMPO DO CONHECIMENTO' AS descricao
					UNION ALL
					SELECT 
						'4' AS codigo,
						'ESCOLAS COM ALUNADO MAIOR DO QUE CENSO' AS descricao
					UNION ALL
					SELECT 
						'5' AS codigo,
						'ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADO PARTICIPANTE' AS descricao
					UNION ALL
					SELECT 
						'6' AS codigo,
						'ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES' AS descricao";
		}
		
		if($_REQUEST['tiporel'] == 2){
			$sql = "SELECT
						'1' AS codigo,
						'ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6' AS descricao
					UNION ALL
					SELECT 
						'2' AS codigo,
						'ESCOLAS COM PST' AS descricao
					UNION ALL
					SELECT 
						'3' AS codigo,
						'ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO' AS descricao
					UNION ALL
					SELECT 
						'4' AS codigo,
						'ESCOLAS COM MAIS DE 4 MACROCAMPOS' AS descricao
					UNION ALL
					SELECT 
						'5' AS codigo,
						'ESCOLAS COM ALUNADO MAIOR DO QUE CENSO' AS descricao
					UNION ALL
					SELECT 
						'6' AS codigo,
						'NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE' AS descricao
					UNION ALL
					SELECT 
						'7' AS codigo,
						'ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADO PARTICIPANTE' AS descricao
					UNION ALL
					SELECT 
						'8' AS codigo,
						'ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES' AS descricao
					UNION ALL
					SELECT 
						'9' AS codigo,
						'ESCOLAS QUE POSSUEM ATIVIDADE TECNOLOGIA DA ALFABETIZAÇÃO' AS descricao";
		}
		
		
		if($_REQUEST['tiporel'] == 3){
			$sql = "SELECT
						'1' AS codigo,
						'ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6' AS descricao
					UNION ALL
					SELECT 
						'2' AS codigo,
						'ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO' AS descricao
					UNION ALL
					SELECT 
						'3' AS codigo,
						'ESCOLAS COM MAIS DE 4 MACROCAMPOS' AS descricao
					UNION ALL
					SELECT 
						'4' AS codigo,
						'NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE' AS descricao
					UNION ALL
					SELECT 
						'5' AS codigo,
						'ESCOLAS COM ALUNADO MAIOR DO QUE CENSO' AS descricao
					UNION ALL
					SELECT 
						'6' AS codigo,
						'ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES' AS descricao";
		}
		
		$db->monta_combo( "tiposit", $sql, 'S', 'Selecione', '', '', '', 415, 'S', '', '', $_REQUEST['tiposit'] );
		?>
	</td>
</tr>
<tr>
	<td align="center" colspan="2" bgcolor="#c0c0c0">
		<input type="button" value="Visualizar" onclick="visualizaEscolas()">
	</td>
</tr>

<?if( $_REQUEST['tiporel'] && $_REQUEST['tiposit'] ){?>
	<tr>
		<td class="SubTituloDireita" valign="top">Justificativa para correção:</td>
		<td>
			<?
				echo campo_textarea('justificativa','S','S','',50,10,250);
			?>
		</td>
	</tr>
	<tr>
		<td align="center" colspan="2" bgcolor="#c0c0c0">
			<input type="button" name="btnfinaliza" value="Enviar para correção - (lista abaixo)" onclick="finalizaEscolas()">
		</td>
	</tr>
<?}?>

</table>

</form>
<?

if($_REQUEST['tiporel'] && $_REQUEST['tiposit']){
	
	//escolas rurais 2012**************************************************************
	if($_REQUEST['tiporel'] == 1){
		
		//--ESCOLAS COM TOTAL DE ATIVIDADES DIFERENTE DE 4
		if($_REQUEST['tiposit'] == 1){
			$sqlin = "select memid 
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
					having count(me.meaid) <> 4";
		}
		
		//--ESCOLAS COM PST
		if($_REQUEST['tiposit'] == 2){
			$sqlin = "select memid 
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
						group by me.memid";
		}
		
		//--ESCOLAS SEM CAMPO DO CONHECIMENTO
		if($_REQUEST['tiposit'] == 3){
			$sqlin = "select me.memid
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
							and mtaid = 676 
							order by 1
							)
						and me.memid not in 
							(
							select distinct a.memid
							from pdeescola.meatividade a 
							inner join pdeescola.metipoatividade b 
								using(mtaid)
							where a.meaano = 2011
							and mtaid = 676 
							order by 1
							)";
		}
		
		//--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
		if($_REQUEST['tiposit'] == 4){
			$sqlin = "select e.memid
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
						and p.mapquantidade > m.mecquantidadealunos";
		}
		
		//--ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
		if($_REQUEST['tiposit'] == 5){
			$sqlin = "select distinct a.memid
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
						having sum(b.mpaquantidade) <> sum(mapquantidade)";
		}
		
		//--ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES
		if($_REQUEST['tiposit'] == 6){
			$sqlin = "Select memid from
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
			                        having count (memid) < 4";
		}
		
		
		$sql = "select '<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'||et.entcodent||'</a>' as acao, 
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
				inner join workflow.estadodocumento esd using (esdid)
				where me.memstatus = 'A'
				and me.memclassificacaoescola = 'R'
				and me.memanoreferencia = 2012
				and esd.esdid = 33
				AND me.memid NOT IN
									(
									--ESCOLAS QUE RECEBERAM RECURSO EM 2011 E NÃO EXECUTARAM ATIVIDADES
									select memid 
									  from (
									select  e.memid AS memid, e2.memid as memid2,
											(select count(*) from pdeescola.meatividade x where e2.memid=x.memid) as total_atv,
											(select count(*) from pdeescola.meatividade xx where e2.memid=xx.memid and  xx.meacomecounoano = false) as total_atv_false
										from pdeescola.memaiseducacao e  
										inner join pdeescola.memaiseducacao e2
											on e.entcodent = e2.entcodent
										inner join pdeescola.meatividade me
											on me.memid = e2.memid 
										where e2.mempagofnde = 't'
										and e.memanoreferencia = 2012
										and e.memclassificacaoescola = 'R'
										and e2.memanoreferencia = 2011
										and e.memstatus = 'A'
										group by e.memid,  e2.memid
									order by e.memid  ) foo 
									    where total_atv = total_atv_false
									)
				AND me.memid in (".$sqlin.")
				order by estuf, mu.mundescricao, entnome";	
		
		
	}
	
	
	
	
	
	
	//escolas urbanas 2012*********************************************************
	if($_REQUEST['tiporel'] == 2){
		
		//--ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6
		if($_REQUEST['tiposit'] == 1){
			$sqlin = "select me.memid
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
					having count(me.meaid) < 5 or count(me.meaid) > 6";
		}
		
		//--ESCOLAS COM PST
		if($_REQUEST['tiposit'] == 2){
			$sqlin = "select memid 
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					left join entidade.entidade et using(entid)
					left join entidade.endereco ed using(entid)
					left join territorios.municipio mu using(muncod)
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					and t.mtaid in (730) 
					group by me.memid";
		}
		
		//--ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO (mtmid = 42)
		if($_REQUEST['tiposit'] == 3){
			$sqlin = "select me.memid
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
					and mtmid = 42 
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
					and mtmid = 42 
					order by 1
					)";
		}
		
		//--ESCOLAS COM MAIS DE 4 MACROCAMPOS
		if($_REQUEST['tiposit'] == 4){
			$sqlin = "select memid
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
					having count(memid) > 4";
		}
		
		//--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
		if($_REQUEST['tiposit'] == 5){
			$sqlin = "select e.memid
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
					and p.mapquantidade > m.mecquantidadealunos";
		}
		
		//--NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE
		if($_REQUEST['tiposit'] == 6){
			$sqlin = "select memid from (
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
						having count(memid) < 5";
		}
		
		//--ESCOLAS COM QUANTIDADE DE ALUNOS NAS ATIVIDADES DIFERENTE DO ALUNADOPARTICIPANTE
		if($_REQUEST['tiposit'] == 7){
			$sqlin = "select distinct a.memid
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
					having sum(b.mpaquantidade) <> sum(mapquantidade)";
		}
		
		//--ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES
		if($_REQUEST['tiposit'] == 8){
			$sqlin = "Select memid from
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
						               and   mme.memclassificacaoescola = 'U'
						               and   mme.memanoreferencia = 2012
						               and   mme.mamescolaaberta = 't'
						            group by mme.memid, eaba.eatid 
						)
						as Total
						            group by memid
						            having count (memid) < 4";
		}
		
		//--ESCOLAS QUE POSSUEM ATIVIDADE TECNOLOGIA DA ALFABETIZAÇÃO
		if($_REQUEST['tiposit'] == 9){
			$sqlin = "select a.memid
					from pdeescola.memaiseducacao a
					inner join pdeescola.meatividade b
						 on a.memid = b.memid
						and a.memanoreferencia = b. meaano
					inner join pdeescola.metipoatividade C
						on b.mtaid = c.mtaid
					inner join pdeescola.metipomacrocampo d
						on c.mtmid = d.mtmid
					where a.memanoreferencia = 2012
					  and a.memclassificacaoescola = 'U'
					  and c.mtasituacao = 'I'
					  and c.mtaid = 674 ";
		}
		
		$sql = "select '<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'||et.entcodent||'</a>' as acao, 
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
				inner join workflow.estadodocumento esd using (esdid)
				where me.memstatus = 'A'
				and me.memclassificacaoescola = 'U'
				and me.memanoreferencia = 2012
				and esd.esdid = 33
				and me.entcodent not in (
						 select entcodent 
				                   from pdeescola.memaiseducacao 
						  where memanoreferencia = 2011 
						    and memstatus = 'A'
						    and memclassificacaoescola = 'U'
							)
				AND me.memid NOT IN
					(
					--ESCOLAS QUE RECEBERAM RECURSO EM 2011 E NÃO EXECUTARAM ATIVIDADES
					--RETIRAR PARA AS VÁLIDAS
				
						select memid 
						  from (
						select  e.memid AS memid, e2.memid as memid2,
								(select count(*) from pdeescola.meatividade x where e2.memid=x.memid) as total_atv,
								(select count(*) from pdeescola.meatividade xx where e2.memid=xx.memid and  xx.meacomecounoano = false) as total_atv_false
							from pdeescola.memaiseducacao e  
							inner join pdeescola.memaiseducacao e2
								on e.entcodent = e2.entcodent
							inner join pdeescola.meatividade me
								on me.memid = e2.memid 
							where e2.mempagofnde = 't'
							and e.memanoreferencia = 2012
							and e.memclassificacaoescola = 'U'
							and e2.memanoreferencia = 2011
							and e.memstatus = 'A'
							group by e.memid,  e2.memid
						order by e.memid  ) foo 
						    where total_atv = total_atv_false 
					) 
				
				and me.memid in ($sqlin)
				
				order by estuf, mu.mundescricao, entnome";	
		
		
	}
	
	
	//escolas urbanas 2011*****************************************************************************
	if($_REQUEST['tiporel'] == 3){
		
		//--ESCOLAS COM TOTAL DE ATIVIDADES IGUAL A 5 OU 6
		if($_REQUEST['tiposit'] == 1){
			$sqlin = "select me.memid
					from pdeescola.meatividade me
					inner join pdeescola.metipoatividade t using(mtaid)
					inner join pdeescola.memaiseducacao e using(memid)
					where e.memstatus = 'A'
					and e.memclassificacaoescola = 'U'
					and e.memanoreferencia = 2012
					and me.meaano = 2012
					group by me.memid
					having count(me.meaid) < 5 or count(me.meaid) > 6";
			
		}
		
		//--ESCOLAS SEM MACRO CAMPO ACOMPANHAMENTO PEDAGÓGICO (mtmid = 42)
		if($_REQUEST['tiposit'] == 2){
			$sqlin = "select me.memid
					from pdeescola.memaiseducacao me
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
						and mtmid = 42 
								)
					and me.memid not in 	(
						select distinct a.memid
						from pdeescola.meatividade a 
						inner join pdeescola.metipoatividade b 
							using(mtaid)
						inner join pdeescola.metipoatividade 
							using(mtmid)
						where a.meaano = 2011
						and mtmid = 42 
								)";
			
		}
		
		//--ESCOLAS COM MAIS DE 4 MACROCAMPOS
		if($_REQUEST['tiposit'] == 3){
			$sqlin = "select memid
					from (
						select mem.memid, mtp.mtmid, mem.entcodent
						  from pdeescola.memaiseducacao mem
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
					) Macro
					group by Macro.memid
					having count(memid) > 4";
			
		}
		
		//--NO MIN. 5 ATIVIDADES DEVEM TER O MESMO Nº DE ALUNOS DO ALUNADO PARTICIPANTE
		if($_REQUEST['tiposit'] == 4){
			$sqlin = "select memid 
					  from (
						select a.memid, qtd1, qtd2
						  from pdeescola.memaiseducacao m
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
					having count(memid) < 5";
			
		}
		
		//--ESCOLAS COM ALUNADO MAIOR DO QUE CENSO
		if($_REQUEST['tiposit'] == 5){
			$sqlin = "select e.memid
					from pdeescola.memaiseducacao e
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
					and p.mapquantidade > m.mecquantidadealunos";
			
		}
		
		//--ESCOLA ABERTA COM MENOS DE 4 ATIVIDADES
		if($_REQUEST['tiposit'] == 6){
			$sqlin = "Select memid from
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
						               and   mme.memclassificacaoescola = 'U'
						               and   mme.memanoreferencia = 2012
						               and   mme.mamescolaaberta = 't'
						            group by mme.memid, eaba.eatid 
						)
						as Total
						            group by memid
						            having count (memid) < 4";
			
		}
		
		
		$sql = "select '<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'||et.entcodent||'</a>' as acao, 
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
				inner join workflow.estadodocumento esd using (esdid)
				where me.memstatus = 'A'
				and me.memclassificacaoescola = 'U'
				and me.memanoreferencia = 2012
				and esd.esdid = 33
				and me.entcodent in (select entcodent 
						       from pdeescola.memaiseducacao 	
						       where memanoreferencia = 2011 
							 and memstatus = 'A' 
							 and memclassificacaoescola = 'U')
				---RETIRAR PARA AS VÁLIDAS
				AND me.entcodent  not in (
					select entcodent 
					  from (
					select  e.memid AS memid, e2.memid as memid2, e.entcodent,
							(select count(*) from pdeescola.meatividade x where e2.memid=x.memid) as total_atv,
							(select count(*) from pdeescola.meatividade xx where e2.memid=xx.memid and  xx.meacomecounoano = false) as total_atv_false
						from pdeescola.memaiseducacao e  
						inner join pdeescola.memaiseducacao e2
							on e.entcodent = e2.entcodent
						inner join pdeescola.meatividade me
							on me.memid = e2.memid 
						where e2.mempagofnde = 't'
						and e.memanoreferencia = 2012
						and e.memclassificacaoescola = 'U'
						and e2.memanoreferencia = 2011
						and e.memstatus = 'A'
						group by e.memid,  e2.memid, e.entcodent
					order by e.memid  ) foo 
					    where total_atv = total_atv_false
				)
				and me.memid in ($sqlin)
				order by estuf, mu.mundescricao, entnome";
		
		
	}
	
	
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

			foreach($dados as $d){
		
				$i++;
				$docid = (integer) $d['docid'];
				$memid = $d['memid'];
				$aedid = 69;
				$cmddsc = $_REQUEST["justificativa"];
				$dadosVerificacao = array("memid"=>$memid);
				/*
				echo '<br><br> i = '.$i;
				echo '<br> docid = '.$docid;
				echo '<br> cmddsc = '.$cmddsc;
				*/
				wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao );
			}
			
			
			echo '<script>
					alert("Operação efetuada com sucesso!");
					location.href="pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A";
				  </script>
				 '; 
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
	if(document.formulario.tiposit.value == ''){
		alert('Campo Obrigatório: Pendência.');
		document.formulario.tiposit.focus();
		return false;
	}
	document.formulario.submit();
	
}


function filtroPendencia (){
	document.formulario.tiposit.value = '';
	document.formulario.submit();
}

function finalizaEscolas(){
	
	
	if(document.formulario.justificativa.value == ''){
		alert('Campo Obrigatório: Justificativa para correção.');
		document.formulario.justificativa.focus();
		return false;
	}
	
	if(confirm('Deseja realmente Enviar para correção (lista abaixo)?')){
		document.formulario.btnfinaliza.value = 'Processando! Aguarde...';
		document.formulario.btnfinaliza.disabled = true;
		document.formulario.varaux.value = '1';
		document.formulario.submit();
	}
}

</script>