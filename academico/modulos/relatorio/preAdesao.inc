<?php
if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file-> getDownloadArquivo($_GET['arquivo']);
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';  
echo "<br>";
monta_titulo('Relatório de Pré-Adesão','');

$sql = "SELECT
					CASE WHEN entsig <> '' THEN
						'<a style=\"cursor:pointer;\" onclick=\"academico_abreSistema( ' || e.entid || ' );\">' || UPPER(entsig) ||  ' - ' || UPPER(entnome) || '</a>'
						ELSE
						'<a style=\"cursor:pointer;\" onclick=\"academico_abreSistema( ' || e.entid || ' );\">' || UPPER(entnome) ||  '</a>' END as nome,
					upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
					case when msminteresse is not null
						then '<center><img src=\"../imagens/check.jpg\"  /></center>'
						else '<center><img src=\"../imagens/unchecked.jpg\"  /></center>'
					end as preadesao,
					coalesce(msmsetorresponsavel,'<center><img src=\"../imagens/unchecked.jpg\"  />') as msmsetorresponsavel,
					case when msm.arqid is not null 
						then '<a href=\"javascript:downloadArquivo(' || msm.arqid || ')\" >' || arq.arqnome || '.' || arq.arqextensao || '</a>'
						else '<center><img src=\"../imagens/unchecked.jpg\"  /></center>'
					end as anexo,
					count(mmrid) as qtde_tutores
				FROM
					entidade.entidade e 
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				LEFT JOIN
					entidade.endereco ed ON ed.entid = e.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ed.muncod
				LEFT JOIN
					academico.maismedicos msm ON e.entid = msm.entid
				LEFT JOIN
					public.arquivo arq ON arq.arqid = msm.arqid
				LEFT JOIN
					academico.maismedicosresponsabilidade res On res.entidunidade = e.entid 
				WHERE
					e.entstatus = 'A' AND ef.funid  in ('12')
				GROUP BY e.entid, e.entnome , e.entsig, mun.mundescricao, mun.estuf, msm.msminteresse, msm.msmsetorresponsavel, msm.arqid, res.entidunidade, arq.arqnome, arq.arqextensao
				ORDER BY
					 e.entsig, e.entnome";
$cabecalho = array('Instituição', 'Município', 'UF','Pré-Adesão','Órgão de Avaliação e Autorização de Pagamento','Anexo do Termo de Pré-Adesão','Qtde. Tutores');
//dbg($sql,1);
$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');
?>
<script>
function downloadArquivo(arqid)
{
	window.location.href="academico.php?modulo=relatorio/preAdesao&acao=C&arquivo="+arqid;
}
function academico_abreSistema( entid ){
	window.location.href = '/academico/academico.php?modulo=principal/dadosentidade&acao=A&entidunidade=' + entid;
}
</script>