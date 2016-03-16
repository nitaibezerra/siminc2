<?php
/************ CONFIGURAÇÃO DE TELA ******************/
$width 			= 'width="600px"';
$bgColor 		= 'bgcolor="silver"';
$numAnexo1	= '5';
$tituloAnexo1	= 'AUTENTICAÇÃO DA DECLARAÇÃO';
$numAnexo2	= '11';
$tituloAnexo2	= 'AUTENTICAÇÃO DAS INFORMAÇÕES';
$nomeDirigente	= 'Teste';

/************ CARREGA DADOS ******************/
$sql = "SELECT  iu.inuid,  
				mun.muncod as id,
				ent.entnumcpfcnpj as cnpj,
				ent.entnome as nomeentidade,
				mun.mundescricao as descricao,
				mun.estuf as estuf
		FROM cte.instrumentounidade 	 iu
		INNER JOIN territorios.municipio mun  ON mun.muncod = iu.muncod
		INNER JOIN entidade.endereco 	 ende ON mun.muncod = ende.muncod AND ende.endstatus = 'A'
		INNER JOIN entidade.entidade  	 ent  ON ende.entid = ent.entid AND ent.entstatus = 'A' 
		INNER JOIN entidade.funcaoentidade fun on fun.entid = ent.entid 
		WHERE iu.inuid = ".$_SESSION['inuid']."
		AND funid = 1";

$arDados = $db->pegaLinha($sql);

$municipio = cte_pegarMuncod( $_SESSION["inuid"] );

$sql = "SELECT distinct
						'' AS acao,
						d.dimcod ||'.'|| ad.ardcod ||'.'|| i.indcod	AS localizacao, 
						s.sbadsc						AS descricaosubacao,
						CASE WHEN s.sbaidpai is not null THEN 'Aditivo' ELSE 'Original' END AS tipo,
						spt.sptplanointerno				AS pi,
						m.estuf 						AS uf,
						'' as valor,
						s.sbaporescola	AS porescola,
						s.sbaid,
						s.sbaid							AS idsubacao,
						a.aciid							AS acao,
						a.acidtinicial 					AS cronogramaexecucaoinicial,
						a.acidtfinal 					AS cronogramaexecucaofinal,
						to_char(a.acidtinicial, 'YYYY')	AS anoinicioexecucao,
						to_char(a.acidtfinal, 'YYYY')	AS anofimexecucao,
						s.sbaid							AS codigodasubacao,
						s.sbadsc						AS descricaosubacao,
						s.sbaporescola					AS cronogramaporescola,
						s.sbacategoria					AS categoriadespesa,
						m.estuf 						AS uf,
						pp.partexto						AS parecer,
						pp.usucpf						AS cpfparecista, 
						su.usunome      				AS nomeparecista,
						spt.sptparecer					AS parecersubacao,
						d.dimcod 						AS codigoDimensao,
						d.dimdsc 						AS descricaoDimensao,
						u.unddsc 						AS descricaounidademedida,
						u.undid 						AS codigounidademedida,
						spt.sptuntdsc                   AS comentarioitens,
						d.dimcod || '&10-- ' || 
						d.dimdsc || '&10-- ' || 
						ad.ardcod || '&10-- ' || 
						ad.arddsc || '&10-- ' || 
						i.indcod || '&10-- ' || 
						i.inddsc || '&10-- ' || 
						a.acidsc || '&10-- ' ||
						s.sbadsc    || '&10-- ' || 
						s.sbastgmpl || '&10-- ' || 
						u.unddsc    || '&10-- ' || 
						coalesce(cast(spt.sptunt as varchar),'0') 	 AS detalhamento,
						sptnumprocesso  AS numeroprocesso,
						spt.sptano AS anodoitens	
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
					INNER JOIN cte.indicador 	  	  i   ON i.ardid  = ad.ardid
					INNER JOIN cte.criterio		  	  c   ON c.indid  = i.indid
					INNER JOIN cte.pontuacao 	  	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
					INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod
					INNER JOIN cte.acaoindicador 	  		a   ON a.ptoid   = p.ptoid
					INNER JOIN cte.subacaoindicador   		s   ON s.aciid   = a.aciid
					INNER JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid and sptano = date_part('year', current_date) and trim(spt.sptplanointerno) <> '' 
					INNER JOIN cte.unidademedida 	  		u   ON u.undid   = s.undid
                    LEFT  JOIN cte.convenioretorno    cvr ON ( cvr.inuid = iu.inuid and spt.sptplanointerno = cvr.prgplanointerno )
					LEFT JOIN cte.parecerpar 	  pp  ON pp.inuid = iu.inuid
					LEFT JOIN seguranca.usuario 	  su  ON su.usucpf 		= pp.usucpf
				WHERE
					 m.muncod = '".$municipio."' AND
					 s.frmid in(11,3,2)  -- assistencia financeira.
					 AND spt.ssuid = 3   -- aprovada pela comissão.
					 AND pp.tppid = 1	 -- parecer financeiro dado
					 AND date_part('year', pp.pardata)= date_part('year', current_date) -- parecer do ano corrente	
				ORDER BY 
					s.sbadsc ";

$dadosPlanos = $db->carregar($sql);

?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../cte/includes/pta.css"/>
	</head>
	<body>
	<!-- ANEXO 1 -->
	<div class="folha">
		<?=anexo1($width,$arDados, $bgColor, $numAnexo1, $tituloAnexo1, $nomeDirigente); ?>
	</div>
	<? echo espacoPlano(); ?>
	
	<!-- ANEXO 2 -->
	<div class="folha">
		<?=anexo2($width,$arDados, $bgColor, $numAnexo2, $tituloAnexo2, $nomeDirigente, $dadosPlanos); ?>
	</div>	
	<? echo espacoPlano(); ?>
	
	<!-- ANEXO 3 -->
	<div class="folha">
		<?=anexo3($width, $arDados, $bgColor, $dadosPlanos); ?>
	</div>	
	
	<!-- ANEXO 4 -->
	<? echo espacoPlano(); ?>
	<div class="folha">
	<? echo anexo4('width="1000px"', $arDados, $bgColor, $dadosPlanos); ?>
	</div>
	
	<!-- ANEXO 5 -->
	<? echo espacoPlano(); ?>
	<div class="folha">
	<? echo anexo5('width="1000px"', $arDados, $bgColor, $dadosPlanos); ?>
	</div>
	
	
	
	</body>
</html>

<?php
function espacoPlano(){
	return '<table align="center" border="0" cellspacing="0" cellpadding="0" style="border-color: black;">
				<tr>
					<td style="height: 50px;">&nbsp;</td>
				</tr>
			</table>';
}

function anexo1($width, $arDados, $bgColor, $numAnexo1, $tituloAnexo1, $nomeDirigente){
?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0" style="border-color: black; border: 2px solid; border-top-color: white;">
		<tr>
			<td>
				<table <?=$width; ?> class="lista" border="1" align="center" <?=$bgColor; ?> cellspacing="1" cellpadding="4" style="border-color: black;">
					<tr>
						<td class="fot" valign="top">MEC / FNDE</td>
						<td valign="top" style="text-align: center;"><span class="fot">PLANO DE TRABALHO</span> <br><b><span style="text-align: center; font-size: 9px">DECLARAÇÃO DE ADIMPLÊNCIA</span></b></td>
						<td class="fot">ANEXO<br>1</td>
					</tr>
				</table>
				<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
					<tr>
						<td valign="top" style="text-align: center;"><b>PLANOS DE AÇOES ARTICULADAS - PAR</b></td>
					</tr>
				</table>
				<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
					<tr <?=$bgColor; ?>>
						<td><b>1 - CNPJ</b></td>
						<td><b>2 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
					</tr>
					<tr>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['cnpj']; ?></td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['nomeentidade']; ?></td>
					</tr>
				</table>
				<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
					<tr <?=$bgColor; ?>>
						<td><b>3 - MUNICÍPIO</b></td>
						<td><b>4 - UF</b></td>
					</tr>
					<tr>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['descricao']; ?></td>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estuf']; ?></td>
					</tr>
				</table>
				<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
					<tr <?=$bgColor; ?>>
						<td><b>DECLARAÇÃO</b></td>
					</tr>
					<tr>
						<td class="textoAnexo1" ><br><br><br><br><center>DECLARAÇÃO DE CUMPRIMENTO DOS CONDICIONANTES LEGAIS</center>
					            <br><br><br><br><br>
					            &nbsp;&nbsp;&nbsp;&nbsp;
					            DECLARO, para fins de prova junto ao FUNDO NACIONAL DE
								DESENVOLVIMENTO DA EDUCAÇÃO – FNDE, para os efeitos e sob as penas
								da lei, que o proponente:
							<br><br>
							a) acha-se em dia quanto ao pagamento de tributos, empréstimos e
							financiamentos devidos à União, inclusive no que concerne às contribuições
							relativas ao PIS/PASEP, de que trata o Art. 239 da Constituição Federal;
							<br><br>
							b) acha-se em dia quanto à prestação de contas de recursos anteriormente recebidos da União;
							<br><br>
							c) fez previsão orçamentária da contrapartida, no valor previsto no
								presente instrumento de transferências voluntárias, conforme § 1º art. 44 da Lei
								nº 11.514, de 13 de agosto de 2007, art. 45, § 1º, - Lei de Diretrizes Orçamentárias.
							<br><br>
							d) dentro dos limites da despesa total com pessoal, em conformidade com o
							disposto no art. 169, § 2º da Constituição Federal c/c art. 25 Parág. 1º IV, “c”, da
							Lei Complementar nº 101/00.
							<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
						</td>
					</tr>
				</table>
				<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
					<tr>
						<td>&nbsp;</td>
					</tr>
				</table>
				<? echo autenticacaoRelatPlanoTrabalho('5','AUTENTICAÇÃO DA DECLARAÇÃO', $arDados, $width, $bgColor);?>
			</td>
		</tr>
	</table>
<? 
}

function anexo2($width, $arDados, $bgColor, $numAnexo2, $tituloAnexo2, $nomeDirigente, $dadosPlanos){
?>
		<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0" style="border-color: black; border: 2px solid; border-top-color: white;">
			<tr><td>
			<table <?=$width; ?> class="lista" border="1" align="center" <?=$bgColor; ?> cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr>
				<td class="fot" valign="top">MEC / FNDE</td>
				<td valign="top" style="text-align: center;"><span class="fot">PLANO DE TRABALHO</span> <br><b><span style="text-align: center; font-size: 9px">DESCRIÇÃO DO PROJETO</span></b></td>
				<td class="fot">ANEXO<br>2</td>
			</tr>
		</table>
		<table <?=$width; ?> align="center" border="1" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr>
				<td valign="top" style="text-align: center;"><b>PLANOS DE AÇOES ARTICULADAS - PAR</b></td>
			</tr>
		</table>
		<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr <?=$bgColor; ?>>
				<td><b>1 - EXERCÍCIO</b></td>
				<td><b>2 - NÍVEL DE ENSINO</b></td>
				<td><b>3 - ABRANGÊNCIA DO PROJETO</b></td>
			</tr>
			<tr>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=date(Y); ?></td>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;QUAL O DADO A SER PEGO</td>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;QUAL O DADO A SER PEGO</td>
			</tr>
		</table>
		<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr <?=$bgColor; ?>>
				<td><b>4 - CNPJ</b></td>
				<td><b>5 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
			</tr>
			<tr>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['cnpj']; ?></td>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['nomeentidade']; ?></td>
			</tr>
		</table>
		<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr <?=$bgColor; ?>>
				<td><b>6 - MUNICÍPIO</b></td>
				<td><b>7 - UF</b></td>
				<td><b>8 - EMENDA Nº</b></td>
			</tr>
			<tr>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['descricao']; ?></td>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estuf']; ?></td>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;QUAL O DADO A SER PEGO</td>
			</tr>
		</table>
		<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr <?=$bgColor; ?>>
				<td><b>9 - AÇÃO A SER EXECUTADA</b></td>
			</tr>
			<tr>
				<td>
				<?PHP
				if($dadosPlanos){
					foreach ($dadosPlanos as $value) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['descricaodimensao']."<br>"; 
				   	}
				}else{
					echo "&nbsp;";
				}
				?>
				</td>
			</tr>
		</table>
		<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr <?=$bgColor; ?>>
				<td><b>10 - JUSTIFICATIVA DO PROJETO</b></td>
			</tr>
			<tr>
				<td style="height: 200px;" valign="top"> QUAL O DADO A SER PEGO</td>
			</tr>
		</table>
		<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
			<tr>
				<td>&nbsp;</td>
			</tr>
		</table>
		<? echo autenticacaoRelatPlanoTrabalho('11','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
			</td>
	</tr>
	</table>
<?php
}

function anexo3($width, $arDados, $bgColor, $dadosPlanos){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0" style="border-color: black; border: 2px solid; border-top-color: white;">
	<tr><td>
	<table <?=$width; ?> class="lista" border="1" align="center" <?=$bgColor; ?> cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td class="fot" valign="top">MEC / FNDE</td>
		<td valign="top" style="text-align: center;"><span class="fot">PLANO DE TRABALHO</span> <br><b>
		                                             <span style="text-align: center; font-size: 9px">DETALHAMENTO DA AÇÃO</span></b></td>
		<td class="fot">ANEXO<br>3</td>
	</tr>
</table>
<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td valign="top" style="text-align: center;"><b>PLANOS DE AÇOES ARTICULADAS - PAR</b></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>1 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
	</tr>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['nomeentidade']; ?></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>2 - MUNICÍPIO</b></td>
		<td><b>3 - UF</b></td>
	</tr>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['descricao']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estuf']; ?></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>4 - AÇÃO A SER EXECUTADA</b></td>
	</tr>
	<tr>
		<?		
		$arPlano = $dadosPlanos;
		?>
		<td>
		<?
		if($arPlano){
			foreach ($arPlano as $value) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['descricaodimensao']."<br>"; 
		   }
		}else{
			echo "&nbsp;";
		}
		 ?></td>
	</tr>
</table>

<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td colspan="4"><b>5 - BENEFICIÁRIOS DA AÇÃO</b></td>
	</tr>
	<tr <?=$bgColor; ?>>
		<td style="text-align: center;"><b>5.1 - BENEFICIÁRIO</b></td>
		<td style="text-align: center;"><b>5.2 - ZONA RURAL</b></td>
		<td style="text-align: center;"><b>5.3 - ZONA URBANA</b></td>
		<td style="text-align: center;"><b>5.4 - TOTAL</b></td>
	</tr>
  <? 
  
  foreach ($dadosPlanos as $dados) { 
  	$beneficiarios = recuperaBeneficiarios($dados['idsubacao']);
	$arBen = $beneficiarios;
  	$i = 0;
  	if($arBen){
		foreach ($arBen as $valor) { $i++;?>
			<tr>
				<td <?=$bgColor; ?>><b>5.1.<?=$i; ?> - <?=$valor['descricaobeneficiario']; ?></b></td>
				<td style="text-align: center;"><b><?=$valor['quantidadezonarural']; ?></b></td>
				<td style="text-align: center;"><b><?=$valor['quantidadezonaurbana']; ?></b></td>
				<td style="text-align: center;"><b><?=$valor['total']; ?></b></td>
			</tr>		 	
	<?	}
  	}else{
  		?>
  		<tr>
  			<td>&nbsp;</td>
  			<td>&nbsp;</td>
  			<td>&nbsp;</td>
  			<td>&nbsp;</td>
  		</tr>
  		<?
  	}
  	}
   ?>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>6 - DETALHAMENTO DA AÇÃO</b></td>
	</tr>
	<?
	$sql = "SELECT 
				pta.ptadescricao 
			FROM emenda.planotrabalho pt INNER JOIN emenda.ptacao pta
				ON (pt.ptrid = pta.ptrid)
			WHERE
				pt.ptrstatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid']."
			ORDER BY pta.ptadescricao";
	
	//$arDetalhe = $db->carregar($sql);
	?>
	<tr>
		<td style="height: 200px;" valign="top">
		<?
		if($arDetalhe){
			foreach ($arDetalhe as $value) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['ptadescricao']."<br>"; 
		   	}
		}else{
			echo "&nbsp;";
		}
		 ?>QUAL O DADO A SER PEGO</td>
	</tr>
</table>
<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td>&nbsp;</td>
	</tr>
</table>

<? echo autenticacaoRelatPlanoTrabalho('7','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
</td></tr></table>
<?
}

function autenticacaoRelatPlanoTrabalho($numAnexo, $tituloAnexo, $arDados, $width, $bgColor){
	$arDados['ptrtipodirigente'] == 'D' ? $tipoDirigente = 'Dirigente' : $tipoDirigente = 'Representante Legal';
	?>
	<table <?=$width ?>  class="lista sembordaBaixa" align="center" cellspacing="1" cellpadding="4" >
			<tr <?=$bgColor; ?> >
				<td colspan="2"><b><?=$numAnexo; ?> - <?=$tituloAnexo; ?></b></td>
			</tr>
		</table>
		<table <?=$width ?> class="lista" align="center" cellspacing="1" cellpadding="4">
			<tr >
				<td class="dtSemBorda"colspan="3">&nbsp;</td>
			</tr>
			<tr>
				<td class="dtSemBorda" style="text-align: center; " colspan="3"><div class="caixa"></div></td>
			</tr>
			<tr>
				<td class="dtSemBorda" style="text-align: center" colspan="3"><b>LOCALIDADE, UF E DATA</b></td>
			</tr>
			<tr>
				<td class="dtSemBorda" colspan="3"></td>
			</tr>
			<tr>
				<td class="dtSemBorda" style="text-align: center;" valign="bottom"><div class="caixa"><b><?=strtoupper($nomeDirigente); ?></b></div></td>
				<td class="dtSemBorda" ></td>
				<td class="dtSemBorda" style="text-align: center;" valign="bottom"><div class="caixa"></div></td>
			</tr>
			<tr>
				<td class="dtSemBorda" style="text-align: center; font-size: 10px;"><b>NOME DO DIRIGENTE OU REPRESENTANTE LEGAL </b></td>
				<td class="dtSemBorda" ></td>
				<td class="dtSemBorda" style="text-align: center; font-size: 10px;"><b>ASSINATURA DO DIRIGENTE OU REPRESENTANTE LEGAL</b></td>
			</tr>
		</table>
		<table <?=$width; ?> class="lista" align="center" cellspacing="1" cellpadding="4" style="border-color: black; ">
				<tr>
					<td colspan="2"><b>Formulário confeccionado obedecendo aos preceitos da IN/STN/MF nº 01, de 15.1.1997 e as suas alterações.</b></td>
				</tr>
		</table>
	<?
}

function recuperaBeneficiarios($codigoSubacao){
		global $db;
		$sql = "SELECT b.benidfnde  AS codigobeneficiario,
					   b.bendsc     AS descricaobeneficiario,
				       sb.vlrurbano AS quantidadezonaurbana ,
				       sb.vlrrural  AS quantidadezonarural ,
				      (sb.vlrurbano + sb.vlrrural ) as total
				FROM cte.subacaobeneficiario sb
				INNER JOIN cte.beneficiario b ON b.benid = sb.benid
				WHERE    sb.sbaid = ".$codigoSubacao." and sb.sabano = date_part('year', current_date)
				ORDER BY sb.benid";
		//dbg($sql,1);
		return  $db->carregar( $sql );
}

function recuperaitensdecomposicao($subacao, $escola, $ano){
		global $db;
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "	somaquantidadeitens 	AS quantidade,
						sum(qtd.qfaqtd)				AS quantidadeglobal,
					  ";
			$conta = "somaquantidadeitens";
			
			$inner = "	INNER JOIN
							--cte.qtdfisicoano qtd ON sba.sbaid = qtd.sbaid AND qtd.qfaano = '$ano'
							cte.qtdfisicoano qtd ON sba.sbaid = qtd.sbaid AND qtd.qfaano = date_part('year', current_date)
						LEFT JOIN 
							(	SELECT cosid, SUM(ecsqtd) as somaquantidadeitens 
								FROM cte.escolacomposicaosubacao 
								GROUP BY cosid) ecs ON cos.cosid = ecs.cosid";
			$groupby = " GROUP BY 
						cos.cosord,
						cos.cosdsc, 		
						cos.cosvlruni,
						ud.unddid,		
						ud.undddsc,
						ecs.somaquantidadeitens,
						cos.cosid	";
			
		}else{ // SQL quando a subação for global
			$select = "	cos.cosqtd 		AS quantidade,
					    spt.sptunt		AS quantidadeglobal,
					  ";
			$conta = "cos.cosqtd";
			
			$inner = "	INNER JOIN
							--cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = '$ano'
							cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = date_part('year', current_date)
							";
							
		} 
		
		$sql="		SELECT 
						".$select."
						cos.cosid 		AS codigo,
						cos.cosord   	AS ordem,
						cos.cosdsc 		AS descricaoitem,
						date_part('year', current_date) as ano,
						cos.cosvlruni 	AS valorunitario,
						ud.unddid		AS codigounidademedida,
						ud.undddsc		AS descricaounidademedida,
						(".$conta." * cos.cosvlruni) as total
					FROM
						cte.subacaoindicador sba
					INNER JOIN
						--cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = '$ano'
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = date_part('year', current_date)
					".$inner."
					INNER JOIN 
						cte.unidademedidadetalhamento ud ON ud.unddid = cos.unddid
					WHERE
					    sba.sbaid =".$subacao. 
					$groupby;
					//if($dados['idsubacao'] == "3514"){
	//if( $subacao  == "2253152"){
		//dbg($sql,1);
	//}
		return  $db->carregar( $sql );
	}



function anexo4($width, $arDados, $bgColor, $dadosPlanos){
	global $db;
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0" style="border-color: black; border: 2px solid; border-top-color: white;">
	<tr><td>
	<table <?=$width; ?> class="lista" border="1" align="center" <?=$bgColor; ?> cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td class="fot" valign="top">MEC / FNDE</td>
		<td valign="top" style="text-align: center;"><span class="fot">PLANO DE TRABALHO</span> <br><b>
		                                             <span style="text-align: center; font-size: 9px">ESPECIFICAÇÃO DA AÇÃO</span></b></td>
		<td class="fot">ANEXO<br>4</td>
	</tr>
</table>
<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES DO FNDE</b></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>1 - EXERCÍCIO</b></td>
		<td><b>2 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
		<td><b>3 - MUNICÍPIO</b></td>
		<td><b>4 - UF</b></td>
	</tr>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=date(Y); ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['nomeentidade']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['descricao']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estuf']; ?></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>5 - AÇÃO A SER EXECUTADA</b></td>
	</tr>
	<tr>
		<?		
		
		
		$arPlano = $dadosPlanos;
		?>
		<td>
		<?
		if($arPlano){
			foreach ($arPlano as $value) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$value['descricaodimensao']."<br>"; 
		   	}
		}else{
			echo "&nbsp;";
		}
		 ?></td>
	</tr>
</table>
<?
$sql = "SELECT 
			e.espnome,
		    e.espunidademedida,
			ptae.ptequantidade,
		    ptae.ptevalorunitario,
		    (ptae.ptequantidade * ptae.ptevalorunitario) as total,
		    ptae.ptevalorproponente,
		  ( CASE WHEN ptae.ptevalorproponente is null THEN (ptae.ptequantidade * ptae.ptevalorunitario)
		         ELSE ((ptae.ptequantidade * ptae.ptevalorunitario) - ptae.ptevalorproponente) END) as concedente
		FROM emenda.planotrabalho pt INNER JOIN emenda.ptacao pta
			ON (pt.ptrid = pta.ptrid) INNER JOIN emenda.ptacaoespecificacao ptae
		    ON (pta.ptaid = ptae.ptaid) INNER JOIN emenda.acaoespecificacao ae
		    ON (ptae.aceid = ae.aceid) INNER JOIN emenda.especificacao e
		    ON (ae.espid = e.espid)
		WHERE pt.ptrstatus = 'A'
			AND ptae.ptestatus = 'A'
		    AND ae.acestatus = 'A'
		    AND e.espstatus = 'A'
		    AND pt.ptrid = ".$arDados['ptrid'];
			

//$arEspecif = $db->carregar($sql);
?>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td rowspan="2" style="text-align: center" width="40%"><b>6 - ESPECIFICAÇÃO DA AÇÃO</b></td>
		<td colspan="2" style="text-align: center" width="30%"><b>6.1 - INDICADOR FÍSICO</b></td>
		<td colspan="2" style="text-align: center" width="30%"><b>6.2 - CUSTO</b></td>
	</tr>
	<tr <?=$bgColor; ?>>
		<td style="text-align: center" width="20%"><b>UNIDADE DE MEDIDA</b></td>
		<td style="text-align: center" width="10%"><b>QUANTIDADE</b></td>
		<td style="text-align: center" width="15%"><b>VALOR UNITÁRIO</b></td>
		<td style="text-align: center" width="15%"><b>VALOR TOTAL</b></td>
	</tr>
	<?
	foreach ($dadosPlanos as $dados) { 
  		$especificacao = recuperaitensdecomposicao($dados['idsubacao'], $dados['cronogramaporescola'], $dados['anodoitens']);
		$arEspecif = $especificacao;
		$valoresGerais =  recuperavaloresgerais($dados['idsubacao'], $dados['cronogramaporescola'], $dados['anodoitens']);

	
	if($arEspecif){
		foreach ($arEspecif as $valor) {
		?>
		<tr>
			<td><?=$dados['descricaosubacao']; ?></td>
			<td><?=$dados['codigounidademedida']; ?></td>
			<td><?=$valor['quantidade']; ?></td>
			<td>R$ <?=number_format($valor['valorunitario'], 2); ?></td>
			<td>R$ <?=number_format($valor['total'], 2); ?></td>
		</tr>
		<?
			$totAcao += $valor['total'];
			$totConcedente+= $valoresGerais[0]['valorconcedente'];
			$totProponente+= $valoresGerais[0]['valorproponente'];
		}
	}
	?>
	<tr>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td <?=$bgColor; ?> colspan="4"><b>7 - TOTAL DA AÇÃO</b></td>
		<td>R$ <?=number_format($totAcao, 2); ?></td>
	</tr>
	<tr>
		<td <?=$bgColor; ?> colspan="4"><b>7.1 - TOTAL DO PROPONENTE</b></td>
		<td>R$ <?=number_format($totProponente, 2); ?></td>
	</tr>
	<tr>
		<td <?=$bgColor; ?> colspan="4"><b>7.2 - TOTAL DO CONCEDENTE</b></td>
		<td>R$ <?=number_format($totConcedente, 2); ?></td>
	</tr>
</table>
<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td>&nbsp;</td>
	</tr>
</table>

<? echo autenticacaoRelatPlanoTrabalho('8','AUTENTICAÇÃO DAS INFORMAÇÕES', $arDados, $width, $bgColor);?>
</td></tr></table>
<?php
}

}

	function recuperavaloresgerais($subacao, $escola, $ano){
		global $db;
		
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "	sum(cos.cosvlruni * ecs.ecsqtd) 		AS cronograma,
						(sum( cos.cosvlruni * ecs.ecsqtd)*0.99) AS valorconcedente,
						(sum(cos.cosvlruni * ecs.ecsqtd)*0.01) 	AS valorproponente,";
			$inner = "INNER JOIN cte.escolacomposicaosubacao ecs ON cos.cosid = ecs.cosid ";
			
		}else{ // SQL quando a subação for global
			$select = "	sum(cos.cosqtd * cos.cosvlruni ) 			AS cronograma,
						(sum(cos.cosqtd * cos.cosvlruni ) * 0.99) 	AS valorconcedente,
						(sum(cos.cosqtd * cos.cosvlruni ) * 0.01) 	AS valorproponente,";
			$inner = "	--INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = '".$ano."'
						INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = date_part('year', current_date)
					";
		}
		
		$sql=	"select ".$select."	
					'$ano' as ano
					from 
						cte.subacaoindicador sba
					INNER JOIN
						--cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = '".$ano."'
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = date_part('year', current_date)
					".$inner."
					where sba.sbaid = ".$subacao."
					group by sba.sbaid";
						//2308341
		//if( $subacao  == "2253152"){
		//dbg($sql,1);
		//}
		return  $db->carregar( $sql );
	}

function anexo5($width, $arDados, $bgColor){
	global $db;
	$sql = "SELECT DISTINCT
				pt.ptrexercicio,
			    p.pronome,
			    te.tpedesc,
			    e.emecodigo
			FROM
			    emenda.planotrabalho pt INNER JOIN emenda.programaemenda pe
			    ON (pt.preid = pe.preid) INNER JOIN emenda.programa p
			    ON (pe.proid = p.proid) INNER JOIN public.tipoensino te
			    ON (p.tpeid = te.tpeid) INNER JOIN emenda.emenda e
			    ON (pe.emeid = e.emeid)
			WHERE
				pt.ptrstatus = 'A'
				AND te.tpestatus = 'A'
			    AND pt.ptrid = ".$arDados['ptrid'];
	
	//$arRegistro = $db->pegaLinha($sql);
	
	$sql_data = "SELECT 
			    to_char(min(ptedatainicio), 'DD/MM/YYYY') as min_dataini, 
			    to_char(max(ptedatafim), 'DD/MM/YYYY') as max_datafim 
			FROM emenda.ptacaoespecificacao ptae INNER JOIN emenda.ptacao pta
			    ON (ptae.ptaid = pta.ptaid) INNER JOIN emenda.planotrabalho pt
			    ON (pta.ptrid = pt.ptrid)
			WHERE ptae.ptestatus = 'A' 
			    AND pt.ptrstatus = 'A'
				AND pt.ptrid = ".$arDados['ptrid'];
	
	//$arData = $db->pegaLinha($sql_data);
	
	$mesIni = explode('/', $arData["min_dataini"]); 
	$mesFim = explode('/', $arData["max_datafim"]); 

	$DataInicio	= retornaMes( $mesIni[1] ) . '/' . $mesIni[2];
	$DataFim	= retornaMes( $mesFim[1] ) . '/' . $mesFim[2];
	
	//calculo timestam das duas datas
	/*$timestamp1 = mktime(0, 0, 0, $mesIni[1], $mesIni[0], $mesIni[2]);
	$timestamp2 = mktime(4, 12, 0, $mesFim[1], $mesFim[0], $mesFim[2]);
	
	//diminuo a uma data a outra
	$segundos_diferenca = $timestamp1 - $timestamp2;
	//echo $segundos_diferenca;
	
	//converto segundos em dias
	$dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
	
	//obtenho o valor absoluto dos dias (tiro o possível sinal negativo)
	$dias_diferenca = abs($dias_diferenca);
	
	//tiro os decimais aos dias de diferenca
	$dias_diferenca = floor($dias_diferenca);
	
	//echo $dias_diferenca;
	
	$quantDias = $dias_diferenca;*/
	
	?>
	<table <?=$width; ?> align="center" cellspacing="0" cellpadding="0" style="border-color: black; border: 2px solid; border-top-color: white;">
	<tr><td>
	<table <?=$width; ?> class="lista" border="1" align="center" <?=$bgColor; ?> cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td class="fot" valign="top">MEC / FNDE</td>
		<td valign="top" style="text-align: center;"><span class="fot">PLANO DE TRABALHO</span> <br><b><span style="text-align: center; font-size: 9px">CRONOGRAMA DE EXECUÇÃO E DESEMBOLSO</span></b></td>
		<td class="fot">ANEXO<br>5</td>
	</tr>
</table>
<table <?=$width; ?> align="center" border="1" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td valign="top" style="text-align: center;"><b>PROGRAMAS E AÇÕES DO FNDE</b></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>1 - EXERCÍCIO</b></td>
		<td><b>2 - NÍVEL DE ENSINO</b></td>
		<td><b>3 - ABRANGÊNCIA DO PROJETO</b></td>
	</tr>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['ptrexercicio']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['tpedesc']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arRegistro['pronome']; ?></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>4 - CNPJ</b></td>
		<td><b>5 - NOME DO ÓRGÃO OU ENTIDADE</b></td>
	</tr>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnumcpfcnpj']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['entnome']; ?></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td><b>6 - MUNICÍPIO</b></td>
		<td><b>7 - UF</b></td>
	</tr>
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['mundescricao']; ?></td>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$arDados['estdescricao']; ?></td>
	</tr>
</table>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td style="text-align: center;" colspan="6"><b>8 - CRONOGRAMA DE EXECUÇÃO</b></td>
	</tr>
	<tr>
		<td <?=$bgColor; ?> width="16%"><b>8.1 - INÍCIO - MÊS/ANO</b></td>
		<td><?=$DataInicio; ?></td>
		<td <?=$bgColor; ?> width="16%"><b>8.2 - TÉRMINO - MÊS/ANO</b></td>
		<td><?=$DataFim; ?></td>
		<td <?=$bgColor; ?> width="16%"><b>8.3 - QUANT. DE DIAS</b></td>
		<td><?=$quantDias; ?></td>
	</tr>											
</table>
<?
	$sql = "SELECT 
			  ptcid,
			  ptrid,
			  ptctipo,
			  to_char(ptcdata, 'DD/MM/YYYY') as ptcdata,
			   trim(to_char(ptcvalor, '999G999G999D99')) as ptcvalor
			FROM 
			  emenda.ptcronogramadesembolso
			WHERE ptctipo = 'C' 
			  AND ptrid = ".$arDados['ptrid']."
			ORDER BY ptcdata";
	
	//$arDadosC = $db->carregar($sql);
	if(is_array($arDadosC)){
	foreach ($arDadosC as $value) {
		switch ($value) {
			case value:
			;
			break;
			
			default:
				;
			break;
		}
		
	}
	}
	

	$sql = "SELECT 
			  ptcid,
			  ptrid,
			  ptctipo,
			  to_char(ptcdata, 'DD/MM/YYYY') as ptcdata,
			   trim(to_char(ptcvalor, '999G999G999D99')) as ptcvalor
			FROM 
			  emenda.ptcronogramadesembolso
			WHERE ptctipo = 'P' 
			  AND ptrid = ".$arDados['ptrid']."
			ORDER BY ptcdata";
	
//	$arDadosP = $db->carregar($sql);
?>
<table <?=$width; ?> class="lista" border="1" align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr <?=$bgColor; ?>>
		<td style="text-align: center;" colspan="6"><b>9 - CRONOGRAMA DE DESEMBOLSO  -  VALORES CONCEDENTE</b></td>
	</tr>	
		<?
		if($arDadosC){
			for($i=1; $i<=12; $i++){
				if($i <= 6 && $i == 1){
					echo '<tr>
							<td '.$bgColor.' style="text-align: center;"><b>JANEIRO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>FEVEREIRO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>MARÇO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>ABRIL</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>MAIO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>JUNHO</b></td>
						</tr>';
				}else if($i > 6 && $i == 7){
					echo '<tr>
							<td '.$bgColor.' style="text-align: center;"><b>JULHO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>AGOSTO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>SETEMBRO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>OUTUBRO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>NOVEMBRO</b></td>
							<td '.$bgColor.' style="text-align: center;"><b>DEZEMBRO</b></td>
						</tr>';
				}
			if(is_array($arDadosC)){
				foreach ($arDadosC as $valor) {
					$mes = explode("/", $valor['ptcdata']);
					
					
					if(strlen( (String) $i) == 1){
						$v = '0'.$i;
					}else{
						$v = $i;
					}
					
					if($mes[1] == $v){
						echo "<td>".$valor['ptcvalor']."</td>";
						break;	
					}else{
						echo "<td>&nbsp;</td>";
						break;	
					}
					
				}
			}

				/*for($i=0; $i<(count($arDadosC) - 6); $i++){
					echo "<td>&nbsp;</td>";		
				}*/
			}
		}else{
		?>
		<tr>		
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<?
		}
		?>
	<tr>
		<td colspan="5" <?=$bgColor; ?>><b>10 - VALOR TOTAL A SER DESEMBOLSADO PELO CONCEDENTE</b></td>
		<td>R$ &nbsp;</td>
	</tr>
	<tr <?=$bgColor; ?>>
		<td colspan="6">&nbsp;</td>
	</tr>
	<tr <?=$bgColor; ?>>
		<td colspan="6" style="text-align: center;"><b>11 - CRONOGRAMA DE DESEMBOLSO  -  VALORES PROPONENTE</b></td>
	</tr>
	<tr>
		<td <?=$bgColor; ?> style="text-align: center;"><b>JANEIRO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>FEVEREIRO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>MARÇO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>ABRIL</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>MAIO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>JUNHO</b></td>
	</tr>		
	<tr>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td <?=$bgColor; ?> style="text-align: center;"><b>JULHO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>AGOSTO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>SETEMBRO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>OUTUBRO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>NOVEMBRO</b></td>
		<td <?=$bgColor; ?> style="text-align: center;"><b>DEZEMBRO</b></td>
	</tr>		
	<tr>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td colspan="5" <?=$bgColor; ?>><b>12 - VALOR TOTAL A SER DESEMBOLSADO PELO PROPONENTE  (valor mínimo de 1%)</b></td>
		<td>R$ &nbsp;</td>
	</tr>
	<tr>
		<td colspan="5" <?=$bgColor; ?>><b>13 - VALOR TOTAL DO PROJETO</b></td>
		<td>R$ &nbsp;</td>
	</tr>
</table>
<table <?=$width; ?> align="center" cellspacing="1" cellpadding="4" style="border-color: black;">
	<tr>
		<td>&nbsp;</td>
	</tr>
</table>

<? echo autenticacaoRelatPlanoTrabalho('14','AUTENTICAÇÃO', $arDados, $width, $bgColor);?>
</td></tr></table>
<?
}

function retornaMes($mes){
	
	switch ($mes){
		case '01' : 
			return 'Jan';
			break;
		case '02' :
			return 'Fev';
			break;
		case '03' :
			return 'Mar';
			break;
		case '04' :
			return 'Abr';
			break;
		case '05' :
			return 'Mai';
			break;
		case '06' :
			return 'Jun';
			break;
		case '07' :
			return 'Jul';
			break;
		case '08' :
			return 'Ago';
			break;
		case '09' :
			return 'Set';
			break;
		case '10' :
			return 'Out';
			break;
		case '11' :
			return 'Nov';
			break;
		case '12' :
			return 'Dez';
			break;
	}
}

?>
