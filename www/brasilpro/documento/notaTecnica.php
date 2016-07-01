<?php


if( !isset( $boGerarNotaTecnica ) ){
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	$db = new cls_banco();
}		

$inuid = $_SESSION["inuid"];

$sql = sprintf("SELECT estdescricao 
				FROM cte.instrumentounidade ins inner join territorios.estado est on ins.estuf = est.estuf 
				WHERE inuid = %d", $inuid);

$estDsc = $db->pegaUm( $sql );

$sql = "select parid from cte.parecer where inuid = ".$inuid;
$numeroParecer = $db->pegaUm($sql);
unset($sql);

$sql = "SELECT 	
			dimcod,
			ardcod,
			indcod,
			valorGlobal + valorPorEscola as valor,
			descricaoSubacao
			--sum(valorGlobal + valorPorEscola) as soma
			-- Global
		FROM (
				SELECT	
					d.dimcod,
					ad.ardcod,
					i.indcod,
					s.sbaid AS idSubacao,
					s.sbadsc AS descricaoSubacao,
					sum(cosvlruni * cosqtd) AS valorGlobal,
					0 as valorPorEscola
					
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
					INNER JOIN cte.indicador i ON i.ardid = ad.ardid
					INNER JOIN cte.criterio c ON c.indid = i.indid
					INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
					INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
					INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	
					INNER JOIN cte.composicaosubacao csa ON csa.sbaid = s.sbaid AND cosano = 2008 -- AND cosano = date_part('year', current_date)
					LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid  AND sptano = 2008 -- AND sptano = date_part('year', current_date)
					
				WHERE
					 iu.inuid = $inuid	
					 AND s.sbaporescola = false -- global.
					 AND spt.ssuid in ( 3, 7 )
					 AND s.frmid in(16,17)
				GROUP BY
					 d.dimcod,
					 ad.ardcod,
					 i.indcod,
					 s.sbaid,
					 s.sbadsc
			UNION ALL
				-- PorEscola
				SELECT	
					d.dimcod,
					ad.ardcod,
					i.indcod,
					s.sbaid AS idSubacao, 
					s.sbadsc AS descricaoSubacao,
					0 AS valorGlobal,
					sum(cosvlruni * cosqtd) AS valorPorEscola	
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
					INNER JOIN cte.indicador i ON i.ardid = ad.ardid
					INNER JOIN cte.criterio c ON c.indid = i.indid
					INNER JOIN cte.pontuacao p ON p.crtid = c.crtid AND p.indid = i.indid
					INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
					INNER JOIN cte.acaoindicador a ON a.ptoid = p.ptoid
					INNER JOIN cte.subacaoindicador s ON s.aciid = a.aciid	
					INNER JOIN cte.composicaosubacao csa ON csa.sbaid = s.sbaid  AND cosano = 2008 -- AND cosano = date_part('year', current_date)
					LEFT JOIN (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON csa.cosid = ecs.cosid
					LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid  AND sptano = 2008 -- AND sptano = date_part('year', current_date)
					
				WHERE
					 iu.inuid = $inuid	
					 AND s.sbaporescola = true -- por escola.
					 AND spt.ssuid in ( 3, 7 )
					 AND s.frmid in(16,17)
				GROUP BY
					 d.dimcod,
					 ad.ardcod,
					 i.indcod,
					 s.sbaid,
					 s.sbadsc
		) AS resultado
		ORDER BY 
				dimcod, 
				ardcod, 
				indcod";
				
$res = $db->carregar( $sql ) ? $db->carregar( $sql ) : array();

// não há controle do locale do servidor...
$mes = array(
	"Janeiro",
	"Fevereiro",
	"Março",
	"Abril",
	"Maio",
	"Junho",
	"Julho",
	"Agosto",
	"Setembro",
	"Outubro",
	"Novembro",
	"Dezembro"
);



$parecer_data      = date( "d" ) . " de " . $mes[date( "m" )-1] . " de " . date( "Y" );

$arConcordancia["da"] = array( "Bahia", "Paraíba" );
$arConcordancia["de"] = array( "Alagoas", "Minas Gerais", "Pernambuco", "Rondônia", "Roraima", "Santa Catarina", "São Paulo", "Sergipe", "Tocantins" );

if( in_array( $estDsc, $arConcordancia["da"] ) ){
	$estCompleto = "<span class='destaque'>Estado da $estDsc</span>";
}
elseif( in_array( $estDsc, $arConcordancia["de"] ) ){
	$estCompleto = "<span class='destaque'>Estado de $estDsc</span>";
}
else{
	$estCompleto = "<span class='destaque'>Estado do $estDsc</span>";
}

$interessadoAtual = "Secretária de Educação do $estCompleto";

?>
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="content-type" content="text/html; charset=utf-8" />
		<title></title>
		<style type="text/css">

			* { margin: 0; font-family: Arial; font-size: 11pt; }
			
			@page { size: 8.5inch 11inch; margin-top: 0.5inch; margin-bottom: 0.7874inch; margin-left: 1.1811inch; margin-right: 0.3291inch }
			
			table { border-collapse: collapse; border-spacing: 0; empty-cells: show; border-color: #fff; }
			
			.cabecalho{ text-align: center; margin-bottom: 30px;  }
			
			.cabecalho .orgao{ font-weight: bold; }
			
			.cabecalho .setor{ font-weight: bold; font-style: italic; }
			
			.paragrafoNormal{ text-indent: 40px; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; }
			
			.divAssinatura{ text-align: center; }
			
			.divTabelaResultado{ margin: 30px 3%; }
			
			.divTabelaResultado td{ padding: 3px; }
			
			.tabelaCabecalho{ text-align: center; background: #ccc; font-weight: bold; }
			
			.divData{ text-align: right; margin: 20px 0 30 0; }
			
			.linhaAssinatura{ margin-bottom: 10px; }
			
			.divAssunto{ margin-bottom: 30px; }
			
			.destaque{ font-weight: bold; }
			
		</style>
	</head>
	
	<body dir="ltr">
		<div style="margin: 10px;">
			
			<div class="cabecalho">
				<img width="80" height="80" src="/imagens/brasao.gif"/>
				<p class="orgao">Ministério da Educação </p>
				<p class="setor">Secretaria de Educação Profissional e Tecnológica</p>
				<p>Diretoria de Articulação e Projetos Especiais</p>
			</div>
			
			<div class="divAssunto">
				<p>NOTA TÉCNICA N.° #NOTATECNICA#</p>
				<p>
					INTERESSADO: <span class="destaque"><?php echo $interessadoAtual; ?></span>
				</p>
				<p>ASSUNTO: <span class="destaque">Plano de Ações Articuladas/Brasil Profissionalizado</span></p>
			<div>
						
			<p class="paragrafoNormal">
				O Governo Federal, por intermédio do Decreto 6.302, de dezembro de 2007, publicado no D.O.U no dia 12 de dezembro de 2007, dispõe sobre a implementação do "PAR/Brasil Profissionalizado", assim, definido em seu Art. 1. "Fica instituído, no âmbito do Ministério da Educação, o Programa Brasil Profissionalizado, com vistas a estimular o Ensino Médio Integrado à educação profissional, enfatizando a educação científica e humanística, por meio da articulação entre formação geral e educação profissional no contexto dos arranjos produtivos e das vocações locais e regionais". 
			</p>
			<p class="paragrafoNormal">
				Considerando os princípios básicos do Plano de Desenvolvimento da Educação - PDE - educação sistêmica, ordenação territorial e desenvolvimento, com foco nos propósitos de melhoria da qualidade da educação no Ensino Médio e na redução de desigualdades relativas às oportunidades educacionais, o Ministério da Educação construiu 64 indicadores, distribuídos em 04 (quatro) Dimensões (Gestão Educacional; Formação de Professores e Profissionais de Serviço e Apoio Escolar; Práticas Pedagógicas e Avaliação; e, Infra-estrutura Física e Recursos Pedagógicos) que nortearam o <?= $estCompleto; ?> na realização do diagnóstico da Educação Básica no Ensino Médio e na Educação Profissional e Tecnológica do sistema local. 
			</p>
			<p class="paragrafoNormal">
				A partir desse diagnóstico, o <?= $estCompleto; ?> elaborou o PAR/Brasil Profissionalizado que visa à expansão do Ensino Médio Integrado a Educação Profissional e Tecnológica no âmbito estadual.
			</p>
			<p class="paragrafoNormal">
				Algumas solicitações do PAR / Brasil Profissionalizado, em questão, são bastante significativas e merecem destaque, a saber:
			</p>
			
			<div class="divTabelaResultado">
				
				<table border="1" width="95%">
					<?php if( count( $res ) ){ ?>
						<tr class="tabelaCabecalho">
							<td width="5%">Indicadores</td>
							<td width="75%">Descrição</td>
							<td width="20%">Valor</td>
						</tr>
						 
						<?php
						$cor = "#eee"; 
						foreach( $res as $arSubacao ){ 
							$cor = $cor == "#fff" ? "#eee" : "#fff";?>
							
							<tr style="background: <?php echo $cor ?>">
								<td align="center"><?php echo $arSubacao["dimcod"].".".$arSubacao["ardcod"].".".$arSubacao["indcod"] ?></td>
								<td><?php echo $arSubacao["descricaosubacao"] ?></td>
								<td align="right">R$ <?php  echo number_format( $arSubacao["valor"], 2, ',', '.' ) ?></td>
							</tr>
						<?php } ?>
					<?php }
					else{ ?>
						<tr class="tabelaCabecalho">
							<td>Não foram encontrados dados.</td>
						<tr>
					<?php } ?>
				</table>
			</div>
			
			<p class="paragrafoNormal">
				Diante do exposto, o Plano de Ações Articuladas / Brasil Profissionalizado do <?= $estCompleto; ?> se enquadra no compromisso assumido pelo MEC e atende às expectativas de cumprimento ao que prevê a legislação da Educação Profissional. Sendo assim, sou de parecer  favorável ao apoio financeiro e de forma  integral.
			</p>
			
			<br />
			
			<div class="divData">
				<p>Brasília, <?= $parecer_data ?></p>
			</div>
			
			<div class="divAssinatura">
				<p class="linhaAssinatura">_______________________________</p>
				<p>Marcelo Camilo Pedra</p>
				<p>Coordenador Geral</p>
				<p>Coordenação Geral de Projetos Especiais</p>
			</div>
			
			<div class="divAssinatura">
				<p class="linhaAssinatura">_______________________________</p>
				<p>Gleisson Cardoso Rubin</p>
				<p>Diretor</p>
				<p>Diretoria de Articulação e Projetos Especiais</p>
			</div>
			
			<p class="paragrafoNormal" style="margin: 40px 0;">
				De acordo com o despacho supra, encaminhe-se o presente processo à DIPRO- FNDE para os trâmites legais. 
			</p>
			
		</div>	
	</body>
</html>