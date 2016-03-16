<?php monta_titulo('Orientações Coordenador Curso',''); ?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" height="200">
	<tr>
		<td align="center" valign="top">
			<br><br>
			<p style="text-align: justify; font-size: 15px; margin-left: 150px;margin-right: 25px;">
			Você está acessando o <span style="font-size:large">SISFOR - Sistema de Gestão e Monitoramento da Formação Continuada do MEC</span>. Seu perfil de acesso é <span style="font-size:large">Coordenador Curso</span>.
			</p>
		</td>
	</tr>
	<tr>
		<td align="center" valign="top">
		<table width="100%">
		<tr>
			<td>
			<?
			include_once APPRAIZ . "includes/library/simec/Grafico.php";
			
			$grafico = new Grafico();
			
			$sql = "select per.pfldsc as descricao, m.mesdsc || ' / ' || fpbanoreferencia as categoria, count(*) as valor
					from sisfor.pagamentobolsista p
					inner join sisfor.tipoperfil t on t.tpeid = p.tpeid
					inner join seguranca.perfil per on per.pflcod = t.pflcod
					inner join sisfor.folhapagamento f on f.fpbid = p.fpbid
					inner join public.meses m ON m.mescod::integer = f.fpbmesreferencia
					where sifid=".$_SESSION['sisfor']['sifid']."
					group by per.pfldsc, m.mesdsc, f.fpbanoreferencia, per.pflnivel, f.fpbid
					order by f.fpbid, per.pflnivel";
			
			$grafico->setTitulo('Pagamentos de bolsas em andamento por perfil/mês')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql);
			
			?>
			</td>
			<td width="25%" valign="top" rowspan="2">
			<p align="center"><b>Saldo de bolsas</b></p>
			<?

			$sql = "select p.pfldsc,
				   epiqtd,
				   (select count(*) from sisfor.pagamentobolsista pg
					inner join sisfor.tipoperfil tt on tt.tpeid = pg.tpeid
					inner join workflow.documento d on d.docid = pg.docid
					where pg.pflcod=p.pflcod and tt.sifid=e.sifid and d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO.") as qtdutilizada
			from sisfor.equipeies e
	 	    inner join seguranca.perfil p on p.pflcod = e.pflcod
	 	    where sifid='".$_SESSION['sisfor']['sifid']."'";
			
			$cabecalho = array('Perfil','Qtd disponível','Qtd utilizada');
			$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%');
			?>
			<p align="center"><b>Dados dos cursistas</b></p>
			<?
			
			$sql_cursistas = "SELECT distinct c.curid FROM sisfor.cursistacurso c
								INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
								WHERE c.sifid='".$_SESSION['sisfor']['sifid']."'";
			
			
			$sql = "SELECT COALESCE(fr.foedesc, 'Não informado') as foedesc, count(*) as tot, round((count(*)::numeric/(select count(*) from ($sql_cursistas) x)::numeric*100),1) FROM sisfor.cursista cc
				LEFT JOIN sisfor.formacaoescolaridade fr ON fr.foeid = cc.curescolaridade
				WHERE cc.curid IN($sql_cursistas)
				GROUP BY fr.foedesc";
			
			$cabecalho = array("Escolaridade","Qtd","%");
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
			
			$sql = "SELECT CASE WHEN cc.cursexo='M' THEN 'Masculino'
							WHEN cc.cursexo='F' THEN 'Feminino'
							ELSE 'Não informado' END as cursexo, count(*) as tot, round((count(*)::numeric/(select count(*) from ($sql_cursistas) x)::numeric*100),1) FROM sisfor.cursista cc
				WHERE cc.curid IN($sql_cursistas)
				GROUP BY cc.cursexo";
			
			$cabecalho = array("Sexo","Qtd","%");
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
			
			$sql = "SELECT
					CASE WHEN extract(year from age(curdatanascimento)) >= 60 THEN '60+'
				     WHEN extract(year from age(curdatanascimento)) >= 50 AND extract(year from age(curdatanascimento)) < 60 THEN '50-60'
				     WHEN extract(year from age(curdatanascimento)) >= 40 AND extract(year from age(curdatanascimento)) < 50 THEN '40-50'
				     WHEN extract(year from age(curdatanascimento)) >= 30 AND extract(year from age(curdatanascimento)) < 40 THEN '30-40'
				     WHEN extract(year from age(curdatanascimento)) >= 20 AND extract(year from age(curdatanascimento)) < 30 THEN '20-30'
					 WHEN extract(year from age(curdatanascimento)) < 20 THEN '19-'
				     ELSE 'Não informado' END as faixa, count(*) as tot, round((count(*)::numeric/(select count(*) from ($sql_cursistas) x)::numeric*100),1)
			FROM sisfor.cursista cc
			WHERE cc.curid IN($sql_cursistas)
			GROUP BY faixa
			ORDER BY faixa";
			
			$cabecalho = array("Faixa etária","Qtd","%");
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
				
			
				
			
			?>
			</td>
		</tr>
		<tr>
			<td>
			<?
			
			$grafico2 = new Grafico();
			
			$sql = "select
						case when cavsituacao='e' then 'Evadido'
						     when cavsituacao='f' then 'Faleceu'
						     when cavsituacao='c' then 'Cursando'
						     when cavsituacao='a' then 'Matriculado'
						     when cavsituacao='b' then 'Desvinculado'
						     when cavsituacao='d' then 'Trancado'
							 when cavsituacao='p' then 'Aprovado'
							 when cavsituacao='r' then 'Reprovado' end as descricao,
						m.mesdsc || ' / ' || fpbanoreferencia as categoria,
						count(*) as valor
						from sisfor.cursistacurso cc
						inner join sisfor.cursistaavaliacoes ca on ca.cucid = cc.cucid
						inner join sisfor.folhapagamento f on f.fpbid = cc.fpbid
						inner join public.meses m ON m.mescod::integer = f.fpbmesreferencia
						where sifid=".$_SESSION['sisfor']['sifid']."
						group by ca.cavsituacao, m.mesdsc, f.fpbanoreferencia, f.fpbid
						order by f.fpbid, descricao";
			
			$grafico2->setTitulo('Situação dos cursistas por mês')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql);
			
			?>
			</td>
		</tr>
		</table>
		</td>
	</tr>
</table>