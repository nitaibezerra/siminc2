<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'reuni/www/funcoes.php';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<!-- biblioteca javascript local -->
<script type="text/javascript">
</script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">

<!-- corpo da página -->

<?php
//limpando a sessão para manter a arvore fechada ao acessar essa página
unset($_SESSION[ 'ArvoreSessaoReuni' ]);
unset($_SESSION['ArvoreSessaoReuniSrc' ]);

$unidade = $db->usuarioUnidadesPermitidas('reuni');

if(count($unidade) == 1)
{
	echo "<script>location.href = 'reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod=".$unidade[0]."' </script>";
}
else
{
	
	$join = $db->usuarioJoinUnidadesPermitidas('reuni');


	$sql = "
	
	select
			
		'<a href=\"reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod='|| u.unicod ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>' as acao,	
	
		 u.unicod  as cod,
		
		 u.unidsc  as unidade,
 		 		  
		 usu.usunome as usuario,

		 max(to_char(monit.mondata, 'DD/MM/YYYY HH24:MI:SS')) as data 
		 
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
		
	left join reuni.monitoramento monit on (monit.unicod = u.unicod)
	left join seguranca.usuario usu on monit.usucpf = usu.usucpf

	left join ( select count(1) as qtdTotal from reuni.pergunta where prgobr = true ) as a ON 1=1
	where  u.unistatus = 'A' and  u.unitpocod = 'U' and u.gunid = 3
	group by u.unicod, u.unidsc, b.qtd, a.qtdTotal , e.esddsc, usu.usunome
	";

	//$db->monta_lista($sql,array("Ação","Código","Unidade","Por","Data"),200,10,'N','','');
	
	$dados = $db->carregar($sql);	
	$dados_array;
	
	/*$sql_total = "select
					count (gp.grpcod) as qtd					
					from reuni.grupopergunta gp
					inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
					inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
					where p.prgord = 2 ";
	
	$total = $db->pegaUm($sql_total);
	*/
	
	foreach($dados as $chave => $val){
		
		$total = 21;        
     
		//montando campo Acompanhamento Situação
		
		$sql_nao_se_aplica = "select count(m.monid) as na from reuni.monitoramento m where unicod = '".$val['cod']."'  and sitcod = 1";
		$nao_se_aplica = $db->pegaUm($sql_nao_se_aplica);
	
		$sql_cor_1 = "select count(m.corcod) as cor1 from reuni.monitoramento m where unicod = '".$val['cod']."' and corcod = 1 and sitcod != 1";
		$cor_1 = $db->pegaUm($sql_cor_1);
		$sql_cor_2 = "select count(m.corcod) as cor2 from reuni.monitoramento m where unicod = '".$val['cod']."' and corcod = 2 and sitcod != 1";
		$cor_2 = $db->pegaUm($sql_cor_2);
		$sql_cor_3 = "select count(m.corcod) as cor3 from reuni.monitoramento m where unicod = '".$val['cod']."' and corcod = 3 and sitcod != 1";
		$cor_3 = $db->pegaUm($sql_cor_3);
		$sql_cor_total = "select count(m.corcod) as cor_total from reuni.monitoramento m where unicod = '".$val['cod']."' and corcod != 0 and sitcod != 1";
		$cor_total = $db->pegaUm($sql_cor_total);	
		
		$total = $total - ($nao_se_aplica);
	
		//$total_preenchido = number_format($cor_total['cor_total']*100/$total,0,'.',',');	
		
		$cor_0 = ($total - ($cor_1 + $cor_2 + $cor_3));
				
		$dados_array[$chave] = array(   "acao" 				=> $val['acao'], 
										"cod" 				=> $val['cod'], 
										"unidade"   		=> $val['unidade'], 
										"acompanhamento"    => monta_grafico_acompanhamento( $cor_0, $cor_1, $cor_2, $cor_3, $total, $nao_se_aplica ),
										"usuario"     		=> $val['usuario'] , 
										"data"    			=> $val['data']);
	}
	                
    $cabecalho = array("Ações", "Código", "Unidade", "Acompanhamento da Situação", "Por", "Data");
        
    $db->monta_lista_array($dados_array, $cabecalho, 50, 20, '', 'center', '');


}
?>
