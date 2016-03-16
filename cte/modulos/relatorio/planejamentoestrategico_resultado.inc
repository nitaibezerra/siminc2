<?php

$condicaoInterno = array();
// uf
if ( isset( $_REQUEST['estcod'] ) && $_REQUEST['estcod'] )
{
	//array_push( $condicaoInterno, " itu.estuf = '" . $_REQUEST['estcod'] . "'" );
	$ultimoNivel = " - uf";
}

// microregião
if ( isset( $_REQUEST['microregioes'] ) && $_REQUEST['microregioes'] )
{
	//array_push( $condicaoInterno, " mic.miccod = '" . $_REQUEST['microregioes'] . "'" );
	$ultimoNivel = " - microregiao";
}
//município
if ( isset( $_REQUEST['municipios'] ) && $_REQUEST['municipios'] )
{
	//array_push( $condicaoInterno, " itu.muncod = '" . $_REQUEST['municipios'] . "'" );
	$ultimoNivel = " - microregiao";
}
// disciplina
if ( isset( $_REQUEST['fodid'] ) && $_REQUEST['fodid'] )
{
	//array_push( $condicaoInterno, " fd.fodid = '" . $_REQUEST['fodid'] . "'" );
	$ultimoNivel = " - disciplina";
}

if(isset($_REQUEST['fodid']) && $_REQUEST['fodid']){
	$nivel1 = "disciplina";
	$nivel2 = "municipio";
	$boEstado = ($_REQUEST["microregioes"]) ? false : true;
	$sqlMun = "
							SELECT
                   				m.muncod as codigo, m.mundescricao as nome, m.estuf as estados
               				FROM territorios.municipio m";
               				if(!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))){
                   			$sqlMun .= 
                   				" INNER JOIN cte.usuarioresponsabilidade ur ON
                       			 	ur.muncod = m.muncod ";
                       		}
               				$sqlMun .= " WHERE ";
							if(!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))){
								$sqlMun .= " ur.usucpf = '" . $_SESSION['usucpf'] . "' AND rpustatus = 'A' AND ";
                   			}
                   			
                   			if($boEstado){
                   				$sqlMun .= " m.estuf = '" . $_REQUEST['estcod'] . "'";
                   			}else{
                   				$sqlMun .= " m.miccod =  " . $_REQUEST['estcod'] . " ";
                   			}
		$sqlMun .= "GROUP BY codigo, nome, estados ORDER BY nome";
		$arNivel2 = $db->carregar( $sqlMun );
}else if(isset($_REQUEST['municipios']) && $_REQUEST['municipios']){
	$nivel1 = "municipio";
	$nivel2 = "disciplina";
	$sqlDis = "SELECT fodid AS codigo, foddsc AS nome FROM cte.formacaodisciplina ORDER BY nome";
	$arNivel2 = $db->carregar( $sqlDis );
}
$ids = "";
if(isset($arNivel2)){
	foreach($arNivel2 as $ar){
		$ids .= "'" . $ar['codigo'] . "',";
	}
}
$ids = substr($ids, 0, -1);
define(MINISTERIO, "Minist&eacute;rio da Educa&ccedil;&atilde;o");
define(PLANO_DESENVOLVIMENTO, "Plano de Desenvolvimento da Educa&ccedil;&atilde;o - PDE");
define(PLANO_ACOES_ARTICULADAS, "Plano de A&ccedil;&otilde;es Articuladas - PAR");
define(PLANO_ESTRATEGICO, "Plano Estrat&eacute;gico - Forma&ccedil;&atilde;o Inicial");
define(MICRO_REGIAO, "Micro-Regi&atilde;o");
define(MUNICIPIOS, "Munic&iacute;pios");

define(GRUPO_DEMANDA_PROFESSORES, "Confirma&ccedil;&atilde;o de Demanda de Professores");
define(GRUPO_RESULTADO_PLANEJAMENTO_ESTRATEGICO, "Resultado do Planejamento Estrat&eacute;gico do Estado, Reunindo a Secretaria de Educa&ccedil;&atilde;o, de Ci&ecirc;ncia e Tecnologia, UNDIME, Reitores das IFES e IES p&uacute;blicas Estaduais e IFET");
define(GRUPO_PROFESSORES_FORMACAO, "E: Professores em Forma&ccedil;&atilde;o de Licenciatura na Disciplina (n&atilde;o dispomos destes dados no Educacenso)");
define(GRUPO_PLANEJAMENTO_OFERTAS, "Planejamento de Ofertas");

define(ID_GRUPO_DEMANDA_PROFESSORES, "1");
define(ID_GRUPO_RESULTADO_PLANEJAMENTO_ESTRATEGICO, "5");
define(ID_GRUPO_PROFESSORES_FORMACAO, "4");
define(ID_GRUPO_PLANEJAMENTO_OFERTAS, "2");

define(SUB_GRUPO_SITUACOES_PERFEITAS_O, "O = Situa&ccedil;&otilde;es Perfeitas: Professores com Licenciatura na sua &Aacute;rea de Atua&ccedil;&atilde;o");
define(SUB_GRUPO_PROFESSORES_SEM_FORMACAO_A, "A = Professores sem forma&ccedil;&atilde;o Superior");
define(SUB_GRUPO_PROFESSORES_COM_LICENCIATURA_B, "B = Professores com Licenciatura mas n&atilde;o sendo na Disciplina");
define(SUB_GRUPO_PROFESSORES_COM_NIVEL_SUPERIOR_C, "C = Professores com N&iacute;vel Superior sem Licenciatura");
define(SUB_GRUPO_DEMANDA_PROFESSOR_TURMA_D, "D = Demanda de Professores pra Atender as Turmas sem Professor");
define(SUB_GRUPO_PREVISAO_FORMACAO_PROFESSORES_F, "F = Previs&atilde;o da Necessidade de Forma&ccedil;&atilde;o de Professores A+B+C+D-E (OBS: nos resultados abaixo do educacenso,n&atilde;o consideramos os itens D e E)");
define(SUB_GRUPO_ESTIMATIVA_APOSENTADORIA_G, "G = Estimativa de aposentadorias = (O+A+B+C)*6/30");
define(SUB_GRUPO_TOTAL_PREVISAO_FORMACAO_PROFESSORES_F_G, "Total de previs&atilde;o para demanda de forma&ccedil;&atilde;o de professores = F + G");
define(SUB_GRUPO_OFERTA_IES, "Oferta Presencial das IES P&uacute;blicas Estaduais");
define(SUB_GRUPO_OFERTA_IFES, "Oferta Atrav&eacute;s de Cursos Presenciais das IFES");
define(SUB_GRUPO_OFERTA_IFET, "Oferta Atrav&eacute;s de Cursos Presenciais dos IFET");
define(SUB_GRUPO_OFERTA_UAB, "Oferta Atrav&eacute;s da UAB");

define(ID_SUB_GRUPO_SITUACOES_PERFEITAS_O, "12");
define(ID_SUB_GRUPO_PROFESSORES_SEM_FORMACAO_A, "1");
define(ID_SUB_GRUPO_PROFESSORES_COM_LICENCIATURA_B, "2");
define(ID_SUB_GRUPO_PROFESSORES_COM_NIVEL_SUPERIOR_C, "3");
define(ID_SUB_GRUPO_DEMANDA_PROFESSOR_TURMA_D, "4");
define(ID_SUB_GRUPO_PREVISAO_FORMACAO_PROFESSORES_F, "11");
define(ID_SUB_GRUPO_ESTIMATIVA_APOSENTADORIA_G, "18");
define(ID_SUB_GRUPO_TOTAL_PREVISAO_FORMACAO_PROFESSORES_F_G, "19");
define(ID_SUB_GRUPO_OFERTA_IES, "17");
define(ID_SUB_GRUPO_OFERTA_IFES, "14");
define(ID_SUB_GRUPO_OFERTA_IFET, "20");
define(ID_SUB_GRUPO_OFERTA_UAB, "15");

define(REDE_ESTADUAL, "Rede Estadual");
define(REDE_MUNICIPAL, "Rede Municipal");

define(ID_REDE_ESTADUAL, "2");
define(ID_REDE_MUNICIPAL, "3");

define(TIPO_INFORMACAO_EDUCACENSO, "Educacenso");
define(TIPO_INFORMACAO_INFORMADO, "informado");

define(ID_TIPO_INFORMACAO_EDUCACENSO, "1");
define(ID_TIPO_INFORMACAO_INFORMADO, "2");

define(OFERTA_EXISTENTE, "Oferta Existente");
define(A_INFORMAR, "a informar");

define(SUB_TOTAL, "Sub-Total");
define(TOTAL, "Total");
define(TOTAL_ULTIMO_NIVEL, TOTAL . $ultimoNivel);

$condicaoInterno = count( $condicaoInterno ) ? " AND " . implode( " AND ", $condicaoInterno ) : "";
//$sql = "
//SELECT  distinct (fl.folvalor), re.renid, re.rendsc, 
//		fl.folvalor as valor, fd.fodid,
//		fd.foddsc as disciplina, ti.tpiid, ti.tpidsc as tipoinformacao, 
//		fsg.fsgid, fsg.fsgdsc, fg.fogid, 
//		fg.fogdsc, mun.miccod as micid, mun.muncod as munid,
//		mun.estuf as estufid, mun.mundescricao, mic.micdsc,
//		fg.fogordem, fsg.fsgordem, re.renordem, fl.fonid
//FROM cte.formacaogrupo fg
//LEFT JOIN cte.formacaosubgrupo fsg 	
//	ON fg.fogid = fsg.fogid
//LEFT JOIN cte.formacaolancamento fl 	
//	ON fsg.fsgid = fl.fsgid
//LEFT JOIN cte.formacaodisciplina fd 	
//	ON fl.fodid = fd.fodid
//LEFT JOIN cte.redeensino re
//	ON fl.renid = re.renid
//LEFT JOIN cte.tipoinformacao ti 		
//	ON fl.tpiid = ti.tpiid
//LEFT JOIN cte.instrumentounidade itu 	
//	ON fl.muncod = itu.muncod
//LEFT JOIN territorios.municipio as mun
//	ON mun.muncod = itu.muncod
//LEFT JOIN territorios.microregiao as mic
//	ON mic.miccod = mun.miccod
//WHERE 
//fl.fonid IS NULL
//AND fsg.fsgstatus = 'A'
//AND fg.fogstatus = 'A'
//AND fd.fodstatus = 'A'" . $condicaoInterno . " order by fg.fogordem, fsg.fsgordem, fd.foddsc, re.renid, ti.tpiid";
/*$sql = "

SELECT DISTINCT (fl.folvalor), re.renid, re.rendsc,
		fl.folvalor as valor, fd.fodid,
		fd.foddsc as disciplina, ti.tpiid, ti.tpidsc as tipoinformacao, 
		fsg.fsgid, fsg.fsgdsc, fg.fogid, 
		fg.fogdsc, mun.miccod as micid, mun.muncod as munid,
		mun.estuf as estufid, mun.mundescricao, mic.micdsc,
		fg.fogordem, fsg.fsgordem, re.renordem, fl.fonid
FROM cte.formacaolancamento fl
LEFT JOIN cte.formacaosubgrupo fsg 	
	ON fsg.fsgid = fl.fsgid
LEFT JOIN cte.formacaogrupo fg
    ON fg.fogid = fsg.fogid
LEFT JOIN cte.formacaodisciplina fd 	
	ON fl.fodid = fd.fodid
LEFT JOIN cte.instrumentounidade itu 	
	ON fl.muncod = itu.muncod
LEFT JOIN territorios.municipio as mun
	ON mun.muncod = itu.muncod
LEFT JOIN territorios.microregiao as mic
	ON mic.miccod = mun.miccod
LEFT JOIN cte.redeensino re
	ON fl.renid = re.renid
LEFT JOIN cte.tipoinformacao ti 		
	ON fl.tpiid = ti.tpiid
WHERE fl.fonid IS NULL
AND fsg.fsgstatus = 'A'
AND fg.fogstatus = 'A'
AND fd.fodstatus = 'A' " . $condicaoInterno . "
ORDER BY fg.fogordem, fsg.fsgordem, fd.foddsc, re.renid, ti.tpiid
";*/

//foreach($arNivel2 as $ar){
$dados[] = carregarDados($ids, $condicaoInterno, $nivel2);
dbg($dados);
//}
function carregarDados($ids, $condicoes, $tipo, $boPedagogia = false){
	global $db;
	$sqlDados = "
		SELECT DISTINCT 
		  fd.foddsc, fg.fogdsc, fs.fsgdsc, fl.folvalor, 
		  mun.estuf, mic.micdsc, mun.mundescricao, re.rendsc, 
		  ti.tpidsc, fg.fogid, fs.fsgid, fg.fogordem, fs.fsgordem
		FROM
			cte.formacaolancamento fl
		LEFT JOIN
			cte.formacaodisciplina fd ON fl.fodid = fd.fodid
		LEFT JOIN
			cte.redeensino re ON re.renid = fl.renid
		LEFT JOIN
			cte.formacaosubgrupo fs ON fl.fsgid = fs.fsgid
		LEFT JOIN
			cte.formacaogrupo fg ON fg.fogid = fs.fogid
		LEFT JOIN
			cte.tipoinformacao ti ON fl.tpiid = ti.tpiid
		LEFT JOIN 
			territorios.municipio as mun ON mun.muncod = fl.muncod
		 LEFT JOIN 
			territorios.microregiao as mic ON mic.miccod = mun.miccod
		WHERE fg.fogstatus = 'A'
		AND fs.fsgstatus = 'A'
		AND ";
		if($boPedagogia){
			$sqlDados .= " fl.fonid IS NOT NULL ";
		}else{
			$sqlDados .= " fl.fonid IS NULL ";
		}
		if($tipo == "municipio"){
			$sqlDados .= " AND fl.muncod in (" . $ids . ")" . $condicoes;
			$ordem = " mun.mundescricao, fd.foddsc, ";
		}else if($tipo == "disciplina"){
			$sqlDados .= " AND fl.fodid in (" . $ids . ")" . $condicoes;
			$ordem = " fd.foddsc, mun.mundescricao, ";
		}
		$sqlDados .= " ORDER BY " . $ordem . " fs.fsgordem, re.rendsc, ti.tpidsc";
		dbg($sqlDados, 1);
		$itens = $db->carregar( $sqlDados );
		$dados = array();
		foreach($itens as $item){
			if($tipo == "disciplina"){
				$descricao = $item['foddsc'];
			}else if($tipo == "municipio"){
				$descricao = $item['mundescricao'];
			}
			$dados[$descricao][]['valor'] = $item['folvalor'];
		}
		unset($itens);
		return $dados;
}

//$sqlDisciplinas = " SELECT fodid AS id, foddsc AS nome FROM cte.formacaodisciplina WHERE fodstatus = 'A' ORDER BY nome";
//$disciplinas = $db->carregar( $sqlDisciplinas );
//$disciplinas = $disciplinas ? $disciplinas : array();
if($_REQUEST['tipo'] == "xls"){
	$cabecalho = array(
		"código", "país","região","estado","município","operadora","laboratorio","possui laboratorio","internet","possui internet","prioridade","codigo prioridade","código da região","sigla da UF"
		,"codigo mesoregião","data da previsão de instalação","data de instalação","area da operadora", "quantidade de escolas","quantidade de aluno","quantidade de professor","quantidade de laboratorio","quantidade da banda larga"
		,"quantidade de banda larga em 2008","quantidade de banda larga em 2009","quantidade de banda larga em 2010"
	);
	$db->sql_to_excel($sql,"BandaLarga",$cabecalho);
	exit;
}
//dbg($sql,1);
//echo($sql);
//$itens = $db->carregar( $sql );
//dbg($itens);
//dump($dados);
//$itens = $itens ? $itens : array();
//echo "<pre>"; print_r($itens); exit;
//$dados = array();
//echo "<pre>";
//print_r($itens); exit;
/*foreach($itens as $item){
	$dados[$item['disciplina']][]['valor'] = $item['valor'];
	$i = count($dados[$item['disciplina']]) - 1;
	$dados[$item['disciplina']][$i]['tipoinformacao'] = $item['tpiid'];
}*/
//echo "<pre>";
//print_r($dados);
//echo "</pre>";
//exit;
//calculando os totais

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<table border="1" cellpadding="4" cellspacing="0">
				<tr>
					<td width="200" align="center"bgcolor="#CCCCCC" colspan="2"><?= MINISTERIO ?></td>
					<td align="center"colspan="6"><?= PLANO_DESENVOLVIMENTO ?></td>
					<td align="center"colspan="6"><?= PLANO_ACOES_ARTICULADAS ?></td>
					<td align="center"colspan="26"><?= PLANO_ESTRATEGICO ?></td>
				</tr>
				<tr>
						<?php if($ultimoNivel == " - disciplina"){ ?>
						<td align="center"bgcolor="#CCCCCC" rowspan="4"><?= DISCIPLINA ?></td>
					<?php }else{ ?>
						<td align="center"bgcolor="#CCCCCC" rowspan="4"><?= MICRO_REGIAO ?></td>
						<td align="center"bgcolor="#CCCCCC" rowspan="4"><?= MUNICIPIOS ?></td>
					<?php } ?>
					<td align="center"bgcolor="#CCCCCC" colspan="18" align="center"><?= GRUPO_DEMANDA_PROFESSORES ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2" rowspan="2"><?= GRUPO_RESULTADO_PLANEJAMENTO_ESTRATEGICO ?></td>

					<td align="center"bgcolor="#CCCCCC" colspan="10"><?= GRUPO_PROFESSORES_FORMACAO ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="8"><?= GRUPO_PLANEJAMENTO_OFERTAS ?></td>
				</tr>
				<tr>
					<td align="center"bgcolor="#CCCCCC" colspan="4"><?= SUB_GRUPO_SITUACOES_PERFEITAS_O ?></td>

					<td align="center"bgcolor="#CCCCCC" colspan="4"><?= SUB_GRUPO_PROFESSORES_SEM_FORMACAO_A ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="4"><?= SUB_GRUPO_PROFESSORES_COM_LICENCIATURA_B ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="4"><?= SUB_GRUPO_PROFESSORES_COM_NIVEL_SUPERIOR_C ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= SUB_GRUPO_DEMANDA_PROFESSOR_TURMA_D ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="4"><?= SUB_GRUPO_PREVISAO_FORMACAO_PROFESSORES_F ?></td>

					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= SUB_GRUPO_ESTIMATIVA_APOSENTADORIA_G ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="4"><?= SUB_GRUPO_TOTAL_PREVISAO_FORMACAO_PROFESSORES_F_G ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= SUB_GRUPO_OFERTA_IES ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= SUB_GRUPO_OFERTA_IFES ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= SUB_GRUPO_OFERTA_IFET ?></td>

					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= SUB_GRUPO_OFERTA_UAB ?></td>
				</tr>
				<tr>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_MUNICIPAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_MUNICIPAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_MUNICIPAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_MUNICIPAL ?></td>
					<td><?= REDE_ESTADUAL ?></td>
					<td><?= REDE_MUNICIPAL ?></td>
					<td><?= REDE_ESTADUAL ?></td>
					<td><?= REDE_MUNICIPAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_MUNICIPAL ?></td>

					<td align="center"bgcolor="#CCCCCC"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= REDE_MUNICIPAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_ESTADUAL ?></td>
					<td align="center"bgcolor="#CCCCCC" colspan="2"><?= REDE_MUNICIPAL ?></td>
					<td bgcolor="#CCCCCC" rowspan="2"><?= OFERTA_EXISTENTE ?></td>
					<td rowspan="2"><?= A_INFORMAR ?></td>
					<td bgcolor="#CCCCCC" rowspan="2"><?= OFERTA_EXISTENTE ?></td>

					<td rowspan="2"><?= A_INFORMAR ?></td>
					<td bgcolor="#CCCCCC" rowspan="2"><?= OFERTA_EXISTENTE ?></td>
					<td rowspan="2"><?= A_INFORMAR ?></td>
					<td bgcolor="#CCCCCC" rowspan="2"><?= OFERTA_EXISTENTE ?></td>
					<td rowspan="2"><?= A_INFORMAR ?></td>
				</tr>
				<tr>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>

					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>

					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>

					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td><?= TIPO_INFORMACAO_INFORMADO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= SUB_TOTAL ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>

					<td><?= SUB_TOTAL ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td>
					<td><?= TOTAL ?></td>
					<td align="center" align="center"align="center"align="center"bgcolor="#CCCCCC"><?= TIPO_INFORMACAO_EDUCACENSO ?></td align="center">

					<td align="center"><?= TOTAL ?></td align="center">
				</tr>
				<tr>
					<?php
						if($ultimoNivel == " - disciplina"){
							$colspan = "";
						}else{
							$colspan = " colspan='2' ";
						}
					?>
					<td bgcolor="lightblue" <?= $colspan ?> align="center"><b><? echo TOTAL_ULTIMO_NIVEL ?></b></td align="center">
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>

					<td bgcolor="lightblue">&nbsp; <b><?= $total ?></b></td>
				</tr>
					<?php foreach($dados as $itens => $value){
						dbg($value, 1);
							echo "<tr>";
								if($ultimoNivel == " - disciplina"){
									echo "<td> Disciplina </td>";
								}else{
									echo "<td> Micro-Regi&atilde;o </td>";
									echo "<td> Munic&iacute;pio </td>";
								}
							foreach($itens as $item){
								$bgColor = ($item['tipoinformacao'] == "1") ? "bgcolor='#CCCCCC'" : "";
								echo "<td align='center' " . $bgColor . ">" . $item['valor'] . "</td>";
							}
							echo "</tr>"; 
						}
						?>
					<?php //} ?>
		</table>