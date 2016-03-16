<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "cte/modulos/principal/pendencia.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "www/cte/_funcoes.php";
/*
$teste = new pendencias("teste",$sub );
$t = $teste->verificaBeneficiarios($sub);
*/

$db = new cls_banco;
$erro = array();

function recuperaPlanointernoAcaoSubacao(){
		global $db;
		
		$municipio = cte_pegarMuncod( $_SESSION["inuid"] );
		if($municipio != NULL){ 		//*** Municipio ***//
			$select  = "m.muncod		AS codigoibge,
					    m.mundescricao	AS nomeentidade,
					    m.mundescricao 	AS nomeMunicipio,";
			$from    = "INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod";
			$where   = "m.muncod = '".$municipio."' --AND"; 
			
		}else{ 							//*** Estado ***//
			$estado	= cte_pegarMuncodEstatual($_SESSION["inuid"]);
			$itrid 	= cte_pegarItrid($_SESSION["inuid"]);  
			
			$select = "m.estdescricao	AS nomeentidade,";
			$from   = "INNER JOIN territorios.estado m ON m.estuf = iu.estuf";
			$where  = "m.muncodcapital 	= '".$estado."' 
					   AND iu.inuid 	= '".$_SESSION["inuid"]."' 
					   AND iu.itrid 	= '".$itrid."' AND";
		}
		
		$sql= "
				SELECT distinct	".$select."
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
						'Renilda'						AS nomeparecista,
						spt.sptparecer					AS parecersubacao,
						pr.prgplanointerno				AS pi,
						d.dimcod 						AS codigoDimensao,
						d.dimdsc 						AS descricaoDimensao,
						u.unddsc 						AS descricaounidademedida,
						u.undid 						AS codigounidademedida,
						spt.sptuntdsc                   AS comentarioitens,
						cvr.cvrnumprocesso 				AS numeroprocesso,
						spt.sptano 						AS anodoitens
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao 	  ad  ON ad.dimid = d.dimid
					INNER JOIN cte.indicador 	  	  i   ON i.ardid  = ad.ardid
					INNER JOIN cte.criterio		  	  c   ON c.indid  = i.indid
					INNER JOIN cte.pontuacao 	  	  p   ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade iu  ON iu.inuid = p.inuid
					".$from."
					INNER JOIN cte.acaoindicador 	  		a   ON a.ptoid   = p.ptoid
					INNER JOIN cte.subacaoindicador   		s   ON s.aciid   = a.aciid
					LEFT JOIN cte.subacaoparecertecnico 	spt ON spt.sbaid = s.sbaid and sptano = date_part('year', current_date)
					INNER JOIN cte.unidademedida 	  		u   ON u.undid   = s.undid
					INNER JOIN cte.programa 	  	  		pr  ON pr.prgid = s.prgid  -- and trim(pr.prgplanointerno) <> ''
					LEFT  JOIN cte.convenioretorno    		cvr ON pr.prgplanointerno = cvr.prgplanointerno and cvr.inuid = iu.inuid
					LEFT JOIN cte.parecerinstrumento 		pi  ON pi.inuid = iu.inuid
					LEFT JOIN cte.parecerpar 	  	  		pp  ON pp.parid = pi.parid AND pp.tppid = 1
					LEFT JOIN seguranca.usuario 	 		su  ON su.usucpf = pp.usucpf
				WHERE
					".$where."
					s.frmid in(11,3)  AND -- assistencia financeira.
					spt.ssuid = 3 	 	--AND -- aprovada pela comissão.
				ORDER BY 
					pr.prgplanointerno,
					d.dimcod";
		return  $db->record_set( $sql );
}


function recuperaitensdecomposicao($subacao, $escola, $ano){
		global $db;
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "	somaquantidadeitens 	AS quantidade,
						sum(qtd.qfaqtd)				AS quantidadeglobal,
					  ";
			
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
						ecs.somaquantidadeitens	";
			
		}else{ // SQL quando a subação for global
			$select = "	cos.cosqtd 		AS quantidade,
					    spt.sptunt		AS quantidadeglobal,
					  ";
			
			$inner = "	INNER JOIN
							--cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = '$ano'
							cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = date_part('year', current_date)
							";
							
		}
		
		$sql="		SELECT 
						".$select."
						cos.cosord   	AS ordem,
						cos.cosdsc 		AS descricaoitem,
						date_part('year', current_date) as ano,
						cos.cosvlruni 	AS valorunitario,
						ud.unddid		AS codigounidademedida,
						ud.undddsc		AS descricaounidademedida
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
		return  $db->carregar( $sql );
	}

function recuperaEscolas($codigoSubacao){
		global $db;
		$sql = "SELECT  e.entnome as nomeEscola , 
					    CASE WHEN substring(e.entcodent, 1, 2) = 'EN' OR length(trim(e.entcodent)) < 8
						THEN '' 
						ELSE e.entcodent END AS codigoInep, --Se o codigo da escola começar com EN (fora da base do inep coloca em branco.)
					    e.entescolanova as escolanova, 
					    coalesce(sum( q.qfaqtd ),0) as quantidadeAlunos
				FROM  cte.qtdfisicoano q
				INNER JOIN entidade.entidade e ON e.entid = q.entid
				where
					q.sbaid = ".$codigoSubacao." AND e.funid = 3
				GROUP BY 
					e.entcodent, 
					e.entnome,
					e.entescolanova";
		//dbg($sql,1);
		return  $db->carregar( $sql );
	}

function recuperaBeneficiarios($codigoSubacao){
		global $db;
		$sql = "SELECT b.benidfnde  AS codigobeneficiario,
					   b.bendsc     AS descricaobeneficiario,
				       sb.vlrurbano AS quantidadezonarural ,
				       sb.vlrrural  AS quantidadezonaurbana
				FROM cte.subacaobeneficiario sb
				INNER JOIN cte.beneficiario b ON b.benid = sb.benid
				WHERE    sb.sbaid = ".$codigoSubacao." and sb.sabano = date_part('year', current_date)
				ORDER BY sb.benid";
		//dbg($sql,1);
		return  $db->carregar( $sql );
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
		//if( $subacao  == "1688776"){
		//dbg($sql);
		//}
		return  $db->carregar( $sql );
	}
	
function processaLinha($dados, &$erro){
	$erroSubacao = $dados['codigodasubacao'];
	$itens = recuperaitensdecomposicao($dados['codigodasubacao'], $dados['cronogramaporescola'], $dados['anodoitens']);
	if(!$dados['parecersubacao']){
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
		$erro['validaErro'][$erroSubacao]['parecer']= 'O parecer técnico na subação não está preenchido';
	}
	if(!$dados['pi']){
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
		$erro['validaErro'][$erroSubacao]['PI']= "O programa selecionado na subação não tem plano interno - PI vinculado. Entrar em contato com o Gestor.";
	}
	if($item['quantidadeglobal'] == 0){
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
		$erro['validaErro'][$erroSubacao]['quantidadesubacao']= 'Obrigatório preencher a quantidade das escolas. ';
				
	}
	if($dados['cronogramaporescola'] == 't'){
		$escolas = recuperaEscolas($dados['codigodasubacao']);
		if($escolas ==false){
			$erro['validaErro'][$erroSubacao]['nome'] 	= $dados['descricaosubacao'];
			$erro['validaErro'][$erroSubacao]['escola'] = "Subação não possui <i>escolas</i>.";	
		}
	}
	$beneficiarios = recuperaBeneficiarios($dados['codigodasubacao']);
	if($beneficiarios != false){
		foreach($beneficiarios as $dadosBeneficiarios){	
			if($dadosBeneficiarios['quantidadezonarural'] == '' && $dadosBeneficiarios['quantidadezonaurbana'] == '' ){
				$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
				$erro['validaErro'][$erroSubacao]['beneficiarios'][] = $dadosBeneficiarios['descricaobeneficiario'].": Os dados não foram preenchidos";
			}
		}
	}else{
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];	
		$erro['validaErro'][$erroSubacao]['beneficiarios'] = "Subação não possui <i>beneficiário</i>.";
	}
	if($dados['descricaosubacao'] == ''){
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
		$erro['validaErro'][$erroSubacao]['descricaosubacao'] = "Subação não possui <i>descrição</i>.";
	}
	if($dados['quantidadesubacao'] == ''){
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
		$erro['validaErro'][$erroSubacao]['quantidadesubacao'] = "Obrigatório preencher a quantidade do cronograma.";
	}
	if($itens != false){
		foreach($itens as $item){
			$codigoItem = $item['ordem'].$item['descricaoitem'];
			$descricaoitem = $item['descricaoitem'];
			if(($item['valorunitario'] == '') ||($item['valorunitario'] == 0)  ){
				$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
				$erro['validaErro'][$erroSubacao]['itemcomposicaovalor'][$descricaoitem] = $item['descricaoitem'];
			}
			if($item['descricaoitem'] == '' ){
				$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
				$erro['validaErro'][$erroSubacao]['descricaoitemcomposicao'] = ": Existem itens de composição sem identificação. (Verifique em  Editar / Inserir Itens de Composição   )";
			}
			if(($item['quantidade'] == '') ||($item['quantidade'] == 0)  ){
				$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
				$erro['validaErro'][$erroSubacao]['itemcomposicao'][$descricaoitem] = $item['descricaoitem'];	
			}
		}
	}else{
		$erro['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
		$erro['validaErro'][$erroSubacao]['itemcomposicao'] = "Subação não possui <i>itens de composição</i>.";
	}

}

function exibeErrosSubAcao($erros) {
	//dbg($erros["parecerGeral"],1);
	echo " <script>
			function alterarSubacao( sbaid ){
				var janela = window.open( \"../cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid=\" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
				janela.focus();
			}
	  	  </script>

		  <div style=\"width : 100%;\">
			<table class=\"tabela\">
			<tr>
			<td colspan='2'>O sistema verificou que alguns dados das seguintes subações não foram preenchidos :</td>
			</tr>";
	/*
		echo "<tr style=\"background-color: #d9d9d9;\">";
		echo "<td colspan='2'>";
		if($dados['parecerGeral']) {
			echo "<b> - ".$erros["parecerGeral"]."</b><br />";
			echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Para poder criar o parecer volte para fase de 'preenchimento de parecer Técnico'</br>";
		}
		echo "</td></tr>";
	*/
	foreach($erros as $idsubacao => $dados) {
		
		if($dados['nome']){
			echo "<tr style=\"background-color: #d9d9d9;\">";
			echo "<td><img src='/imagens/consultar.gif' onclick='alterarSubacao(". $idsubacao .")'></td>";
			echo "<td><b>". $dados['nome'] ."</b><br />";
			if($dados['parecer']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['parecer']."<br />";
			}
			if($dados['PI']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['PI']."<br />";
			}
			if($dados['escola']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['escola']."<br />";
			}
			if($dados['quantidadesubacao']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['quantidadesubacao']."<br />";
			}
			if($dados['descricaosubacao']) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$dados['descricaosubacao']."<br />";
			}
			if(($dados['descricaoitemcomposicao']) ||($dados['itemcomposicao']) ) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>Erros nos itens de composição:</i><br />";
				if($dados['descricaoitemcomposicao']) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Existem itens de composição sem identificação. (Verifique em  Editar / Inserir Itens de Composição)</br>";
				}
				if($dados['itemcomposicaovalor']) {
					if(!is_array($dados['itemcomposicaovalor'])) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['itemcomposicaovalor'] ." ";
					} else {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;O valor unitario dos itens abaixo não podem ser Zero:</br>";
						foreach($dados['itemcomposicaovalor'] as $item) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$item."<br />";
						}
					}
					echo"<br />";
				}
				if($dados['itemcomposicao']) {
					if(!is_array($dados['itemcomposicao'])) {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['itemcomposicao'] ."<br />";
					} else {
						echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A quantidade dos itens abaixo não podem ser Zero:</br>";
						foreach($dados['itemcomposicao'] as $item) {
							echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$item."<br />";
						}
					}
					echo"<br />";
				}
			}
		}
		if($dados['beneficiarios']) {
			echo "<br />";
			if(!is_array($dados['beneficiarios'])) {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ". $dados['beneficiarios'] ."<br />";
			} else {
				echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<i>Erros nos beneficiários:</i><br />";
				foreach($dados['beneficiarios'] as $benef) {
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - ".$benef."<br />";
				}
			}
		}
		echo "</td></tr>";
	}
	echo "</table>";
}


$recurso 	= recuperaPlanointernoAcaoSubacao();
$total		= $db->conta_linhas($recurso) + 1;
for ( $linha = 0; $linha < $total; $linha++ ){
	$dados = $db->carrega_registro( $recurso, $linha );
	/*
	if(!$dados['parecer']){
		$erro['validaErro']['parecerGeral']= 'O parecer Técnico não foi preenchido';
	}
*/
	processaLinha($dados,$erro);
}

exibeErrosSubAcao($erro['validaErro']);
exit;


?>