<?php

ini_set('memory_limit','1024M');

if($_POST['carregaDados'] == 'plano'){
	
	$sql = "SELECT
				t.estdescricao AS descricao,
				count(*) as total,						
				p.esdid,
				d.estuf
			FROM pdeescola.preenchimento p
			INNER JOIN workflow.estadodocumento e ON p.esdid = e.esdid					
			INNER JOIN entidade.endereco d ON p.entid = d.entid
			INNER JOIN territorios.estado t ON upper(d.estuf) = upper(t.estuf)
			WHERE p.esdid = {$_POST['esdid']}
			AND p.parcela ilike '%--%' 
			GROUP BY p.esdid, t.estdescricao, d.estuf
			ORDER BY t.estdescricao";			
	
	$arParamentros = array('esdid','estuf');
	montaListaSubitensPainelEscola($sql, $arParamentros);
	die();
}

if($_POST['carregaDados'] == 'parcelaComplementar'){
	
	$sql = "SELECT
				t.estdescricao as descricao,
				count(*) as total,
				'true' as parcela,
				t.estuf,
				p.esdid
			FROM pdeescola.preenchimento p
			INNER JOIN entidade.endereco e ON p.entid = e.entid
			INNER JOIN territorios.estado t ON upper(e.estuf) = upper(t.estuf)			
			WHERE p.entcodent is not null
			AND p.parcela not ilike '%--%'
			AND p.esdid = {$_POST['esdid']}
			GROUP BY t.estuf, t.estdescricao, p.esdid
			ORDER BY t.estdescricao";
	
	$arParamentros = array('parcela','estuf','esdid');
	montaListaSubitensPainelEscola($sql, $arParamentros);
	die();
	
}

if($_POST['carregaDados'] == 'autoavaliacao'){
	
	$sql = "SELECT 
				t.estdescricao as descricao,
				count(* ) as total,
				t.estuf,
				'true' as pontuacao
			FROM pdeescola.preenchimento p
			INNER JOIN entidade.endereco e ON p.entid = e.entid
			INNER JOIN territorios.estado t ON upper(e.estuf) = upper(t.estuf)
			WHERE pontuacao NOT ILIKE '%--%'
			GROUP BY t.estdescricao, t.estuf
			ORDER BY t.estdescricao";
	
	$arParametros = array('pontuacao','estuf');
	montaListaSubitensPainelEscola($sql, $arParametros);
	die();
}

//Monta o cabeçalho e título da tela
include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Painel", "PDE Escola");

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>

function carregaDadosAjax(idConteudo, idArvore, stParamentros){
		
	new Ajax.Request('pdeescola.php?modulo=painel_pdeescola&acao=A',{

		method: 'post',
		parameters: 'carregaDados='+idArvore+stParamentros,
		onComplete: function(res){
		
			$('imgMais_'+idConteudo).style.display = 'none';
			$('imgMenos_'+idConteudo).style.display = 'block';
			
			$(idConteudo).innerHTML = res.responseText;
			$('tr_'+idConteudo).style.display = '';	
		
		}
	});	

}

function ocultarDadosAjax(idConteudo){
			
	$('imgMais_'+idConteudo).style.display = 'block';
	$('imgMenos_'+idConteudo).style.display = 'none';	
	$('tr_'+idConteudo).style.display = 'none';	
	
}

</script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/pdeescola/pdeescola.php?modulo=lista&acao=E'">PDE - Escola (Plano)</a>
		</th>
	</tr>
	<tr>
		<td valign="top">
			<?php
			
			$sql = "SELECT
						CASE WHEN e.esddsc IS NOT NULL THEN e.esddsc
						ELSE 'Não Iniciado'
						END AS descricao,
						count(*) as total,						
						p.esdid,
						'false' as parcela							
					FROM pdeescola.preenchimento p
					INNER JOIN workflow.estadodocumento e ON p.esdid = e.esdid					
					WHERE e.esdid NOT IN (142, 143, 144, 145)
					AND p.parcela ilike '%--%'
					GROUP BY e.esddsc, e.esdordem, p.esdid
					ORDER BY e.esdordem";
			
			$arParametros = array('esdid', 'parcela');
			if( pdeescola_possui_perfil(PDEESC_PERFIL_CONSULTA)  || $db->testa_superuser() ){			
				montaListaPainelEscola($sql, 'plano', $arParametros);
			}

			?>			
		</td>
	</tr>		
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/pdeescola/pdeescola.php?modulo=lista&acao=E'">PDE - Escola (Parcela complementar)</a>
		</th>
	</tr>
	<tr>
		<td valign="top">
			<?php

			$sql = "SELECT * FROM (
					SELECT
						e.esddsc AS descricao,									
						count(*) as total,
						'true' as parcela,
						e.esdid,
						e.esdordem						
					FROM pdeescola.preenchimento p
					INNER JOIN workflow.estadodocumento e ON p.esdid = e.esdid
					WHERE p.parcela not ilike '%--%'
					AND e.esdid IN (142, 143, 144, 145, 146)
					GROUP BY e.esddsc, e.esdordem, e.esdid
					
					UNION ALL
					
					SELECT
						'Em elaboração - Parcela complementar' AS descricao,									
						count(*) as total,
						'true' as parcela,
						p.esdid,
						20 as esdordem	
					FROM pdeescola.preenchimento p
					--INNER JOIN pdeescola.planoacaosegundaparcela s ON p.pdeid = s.pdeid
					WHERE p.parcela not ilike '%--%'
					AND p.esdid IN (90)
					GROUP BY p.esdid
					
					) AS Parcela
					ORDER BY esdordem";
						
			$arParamentros = array('parcela', 'esdid');
			if( pdeescola_possui_perfil(PDEESC_PERFIL_CONSULTA)  || $db->testa_superuser() ){
				montaListaPainelEscola($sql, 'parcelaComplementar', $arParamentros);
			}

			?>								
		</td>
	</tr>
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/pdeescola/pdeescola.php?modulo=lista&acao=E'">PDE - Escola (Autoavaliação)</a>
		</th>
	</tr>
	<tr>
		<td valign="top">
			<?php
			
			$sql = "SELECT 
						'Autoavaliação' as descricao,
						count(* ) as total,
						'true' as pontuacao
					FROM pdeescola.preenchimento 
					WHERE pontuacao NOT ILIKE '%--%'
					";
			
			$arParamentros = array('pontuacao');
			if( pdeescola_possui_perfil(PDEESC_PERFIL_CONSULTA)  || $db->testa_superuser() ){
				montaListaPainelEscola($sql, 'autoavaliacao', $arParamentros);
			}

			?>
		</td>
	</tr>
</table>