<?php

// monta cabeçalho
//include APPRAIZ . 'includes/cabecalho.inc';
//print '<br/>';
//$db->cria_aba( $abacod_tela, $url, '' );
//monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<!-- biblioteca javascript local -->
<script type="text/javascript">
</script>
<style>
body, td, pre {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 10px;
}
</style>
<!-- corpo da página -->
<?php

function limpaTags($string){
	
	
	$string = eregi_replace("<p[^>]*>", "", $string);
	
	$string  = str_replace('<!--[if !supportLineBreakNewLine]-->','',$string);
	                        
	$string  = str_replace('<!--[if !supportLists]-->','',$string);
	$string  = str_replace('&lt;!--[if !supportLists]--&gt;','',$string);
	
	$string  = str_replace('<!--[endif]-->','',$string);
	$string  = str_replace('&lt;!--[endif]--&gt;','',$string);
	//$string  = strip_tags($string , '<table> <tr> <td> <br> <b> <strong>' );
	
	
	return $string;
}


function montaVersaoParaEmpressao($cod){
	global $db;
	//1
	$sql = "select unpend,unpdir,unpcar,unpsum from reuni.unidadeproposta where unicod = '".$cod."'";
	$dado = $db->pegaLinha($sql);


	$endereco 		= nl2br($dado['unpend']);
	$dirigente 		= nl2br($dado['unpdir']);
	$caracteristica = nl2br($dado['unpcar']);
	$sumula 		= nl2br($dado['unpsum']);


	// 2 3 4 5 6 7 8

	$sql = "
			select
			codPai,
			dscPai,
			ordemPai,
			codFilho,
			dscFilho,
			ordemFilho,
			codPergunta,
			dscPergunta,
			ordemPergunta,
			obrPergunta,
			qtdCaracteres,
			resposta
			from
			(
			select
			gp.grpcod as codPai,
			gp.grpdsc as dscPai,
			gp.grpord as ordemPai,
			gp2.grpcod as codFilho,
			gp2.grpdsc as dscFilho,
			gp2.grpord as ordemFilho,
			p.prgcod as codPergunta,
			p.prgdsc as dscPergunta,
			p.prgord as ordemPergunta,
			p.prgobr as obrPergunta,
			p.prgqtdcar as qtdCaracteres,
			coalesce(r.rspdsc,'<b>Não respondido</b>') as resposta
			from reuni.grupopergunta gp
			inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
			inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
			left join (Select p.prgcod, r.unicod, r.unitpocod, coalesce(r.rspdsc,'') as rspdsc
			from reuni.pergunta p
			inner join reuni.resposta r
			ON r.prgcod = p.prgcod
			inner join public.unidade u
			ON u.unicod = r.unicod and u.unitpocod = r.unitpocod where u.unicod = '".$cod."'
			) as r on r.prgcod = p.prgcod
			where gp.prjcod = 1
			
			union all
			select
			gp.grpcod as codPai,
			gp.grpdsc as dscPai,
			gp.grpord as ordemPai,
			null as codFilho,
			null as dscFilho,
			null as ordemFilho,
			p.prgcod as codPergunta,
			p.prgdsc as dscPergunta,
			p.prgord as ordemPergunta,
			p.prgobr as obrPergunta,
			p.prgqtdcar as qtdCaracteres,
			coalesce(r.rspdsc,'<b>Não respondido</b>') as resposta
			from reuni.grupopergunta gp
			inner join reuni.pergunta p ON p.grpcod = gp.grpcod
			left join (Select p.prgcod, r.unicod, r.unitpocod, coalesce(r.rspdsc,'') as rspdsc
			from reuni.pergunta p
			inner join reuni.resposta r
			ON r.prgcod = p.prgcod
			inner join public.unidade u
			ON u.unicod = r.unicod and u.unitpocod = r.unitpocod where u.unicod = '".$cod."'
			) as r on r.prgcod = p.prgcod
			where gp.grpcodfil is null and gp.prjcod = 1
			) as foo
			order by
			ordemPai,
			ordemFilho,
			ordemPergunta
			";

	$result = $db->carregar($sql);
	
	$saida .= '<table style="width:790px;max-width:790px" bgcolor="#ffffff" cellSpacing="1" cellPadding="3" align="center">';
	$saida .= '<tr>';
	$saida .= '<td align="center">';
	$saida .= '<b>ANEXO I - FORMULÁRIO DE APRESENTAÇÃO DE PROPOSTAS</b><br>';
	$saida .= 'Impresso em: ' . date('d/m/Y - h:i' );
	$saida .= '<p>&nbsp</p>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left">';
	$saida .= '<b><font color="#000000"size="4"> 1. Dados da universidade</font></b>';
	$saida .= '<p>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 10px">';
	$saida .= '<font color="#000000"  size="2">Nome da Universidade:</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 20px">';
	$saida .= '<font color="#888888"  size="2">'.nomeUnidade($cod).'</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 10px">';
	$saida .= '<font color="#000000"  size="2">Endereço:</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 20px">';
	$saida .= '<font color="#888888"  size="2">'.$endereco.'</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 10px">';
	$saida .= '<font color="#000000"  size="2">Dirigente:</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 20px">';
	$saida .= '<font color="#888888"  size="2">'.$dirigente.'</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 10px">';
	$saida .= '<font color="#000000"  size="2">Característica Atual da Instituição:</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 20px">';
	$saida .= '<font color="#888888"  size="2">'.$caracteristica.'</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 10px">';
	$saida .= '<font color="#000000"  size="2">Súmula do Plano:</font>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$saida .= '<tr>';
	$saida .= '<td align="left" style="padding-left: 20px">';
	$saida .= '<font color="#888888"  size="2">'.$sumula.'</font>';
	$saida .= '</td>';
	$saida .= '</tr>';


	$saida .= '<tr>';
	$saida .= '<td align="left">';
	$saida .= '<b><font color="#000000"  size="4">2. As dimensões do plano de reestruturação</font></b>';
	$saida .= '</td>';
	$saida .= '</tr>';

	$i = 0;
	while ($i < count($result))
	{
		$saida .= '<tr>';


		if($result[$i]['codpai'] >= 28) {
			$saida .= '<td>';
			$saida .= '<p>&nbsp;</p>';
			$saida .= '<b><font color="#000000"  size="4">'. $result[$i]['dscpai'] . '</font></b>';
		}else{
			$saida .= '<p>';
			$saida .= '<td style="padding-left: 10px">';
			$saida .= '<b>'. $result[$i]['dscpai'] . '</b>';
		}
		$saida .= '</td>';
		$saida .= '</tr>';


		$codPai     = $result[$i]['codpai'];
		$codPaiNovo = $result[$i]['codpai'];

		$t=1;
		while($codPai == $codPaiNovo )
		{
			$codFilho = $result[$i]['codfilho'];
			$codFilhoNovo = $codFilho;

			if($codFilho != '')
			{

				$saida .= '<tr>';
				$saida .= '<td style="padding-left: 20px">';
				$saida .= '<b>' . $result[$i]['dscfilho'] . '</b>';
				$saida .= '<p>';
				$saida .= '</td>';
				$saida .= '</tr>';

				$j = 1 ;
				while($codFilhoNovo == $codFilho  )
				{

					$saida .= '<tr>';
					$saida .= '<td style="padding-left: 30px">';
					$saida .= '<font color="#000000"  size="2">'. $j .'.&nbsp;' .  $result[$i]['dscpergunta'] . ':</font>' ;
					$saida .= '</td>';
					$saida .= '</tr>';

					$saida .= '<tr>';
					$saida .= '<td style="padding-left: 50px">';
					$saida .= '<font color="#888888"  size="2">' . limpaTags(nl2br($result[$i]['resposta'])). '</font>' ;
					$saida .= '</td>';
					$saida .= '</tr>';

					$j++;
					$i++;

					$codFilhoNovo = $result[$i]['codfilho'];
				}
				//$i++;
				$codPaiNovo = $result[$i]['codpai'];
			}else{

				$saida .= '<tr>';
				$saida .= '<td style="padding-left: 10px">';
				$saida .= $t .'.&nbsp;' .  $result[$i]['dscpergunta'] . ':' ;
				$saida .= '</td>';
				$saida .= '</tr>';

				$saida .= '<tr>';
				$saida .= '<td style="padding-left: 30px">';
				$saida .= '<font color="#888888"  size="2">' . nl2br($result[$i]['resposta']). '</font>' ;
				$saida .= '</td>';
				$saida .= '</tr>';

				$t++;
				$i++;
				$codPaiNovo = $result[$i]['codpai'];
			}
		}
	}

	$saida .= '</table>';
	return limpaTags($saida);
}



$cod = base64_decode($_REQUEST['unicod']);
echo montaVersaoParaEmpressao($cod);
?>