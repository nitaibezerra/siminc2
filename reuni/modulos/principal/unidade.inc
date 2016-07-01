<?php

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<!-- biblioteca javascript local -->
<script type="text/javascript">
</script>

<!-- corpo da página -->

<?php


$unidade = $db->usuarioUnidadesPermitidas('reuni');

if(count($unidade) == 1)
{
	echo "<script>location.href = 'reuni.php?modulo=principal/cadastro&acao=C&unicod=".$unidade[0]."' </script>";
}
else
{
	
	$join = $db->usuarioJoinUnidadesPermitidas('reuni');


	$sql = "
	
	select
			
		'<a href=\"reuni.php?modulo=principal/cadastro&acao=C&unicod='|| u.unicod ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>' as acao,	
	
		 u.unicod  as cod,
		
		 u.unidsc  as unidade,
		 
		 coalesce(e.esddsc,'Não iniciado') as status,
		 
 		 		  
		'<center><span style=\"color:cococo;font-size: 10px;\">' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal)) || '%</span>
		 <div style=\"text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;\" title=\"' ||  floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%\">
		 <div style=\"font-size:4px;width: ' || floor((coalesce(b.qtd,0) * 100)/(a.qtdTotal))  || '%; height: 6px; max-height: 6px; background-color:#339933;\">
		 </div></div></center>' 
		 
		 as porcentagem
		 
			
	
	
	from public.unidade as u $join and u.unicod = unijoin.unicod and  unijoin.unitpocod = u.unitpocod

	left join reuni.unidadeproposta up ON up.unicod = u.unicod and up.unitpocod = u.unitpocod
	left join reuni.proposta p ON p.prjcod = up.prjcod
	
	left join workflow.documento d on d.docid = up.docid
	left join workflow.estadodocumento e on e.esdid = d.esdid
	
	left join
	(select u.unicod, u.unitpocod, count(1) as qtd from reuni.resposta r
	inner join public.unidade u ON u.unicod = r.unicod and u.unitpocod = r.unitpocod
	where char_length(r.rspdsc) > 0 and u.unistatus = 'A'
	group by u.unicod, u.unitpocod
	) as b on b.unicod = u.unicod and b.unitpocod = u.unitpocod

	left join ( select count(1) as qtdTotal from reuni.pergunta where prgobr = true ) as a ON 1=1
	where  u.unistatus = 'A' and  u.unitpocod = 'U' and u.gunid = 3
	group by u.unicod, u.unidsc, b.qtd, a.qtdTotal , e.esddsc
	";



	
	$db->monta_lista($sql,array("Ação","Código","Unidade","Status","% Preenchimento"),200,10,'N','','');


}
?>


