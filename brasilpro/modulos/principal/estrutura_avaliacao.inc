<?php
cte_verificaSessao();


require_once APPRAIZ . "adodb/adodb.inc.php";

require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";

require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

include_once( APPRAIZ. "brasilpro/classes/EntidadeDetalhe.class.inc" );

//ver($_SESSION['inuid']);





$obEntidaDedetalhe = new EntidadeDetalhe();

$estado_documento = wf_pegarEstadoAtual( cte_pegarDocid( $_SESSION['inuid'] ) );

$boPossuiPerfil = cte_possuiPerfilExclusivo( array( CTE_PERFIL_EQUIPE_LOCAL, CTE_PERFIL_EQUIPE_LOCAL_APROVACAO ) );



if( ( $estado_documento['esdid'] == CTE_ESTADO_DIAGNOSTICO || $estado_documento['esdid'] == CTE_ESTADO_PAR ) && $boPossuiPerfil ) {

	if( $obEntidaDedetalhe->existePendenciaEPTPorInuid( $_SESSION["inuid"] ) ){

		echo "<script>location.href = '?modulo=principal/relacionarOfertaEPT&acao=A';</script>";

	}

}





if ($_REQUEST['titleFor']) {

    $sql = '

    SELECT

        CASE WHEN

            sa.sbaporescola <> false THEN \'Por escola\'

        ELSE

            \'Global\'

        END as sbaporescola,

        usu.usunome,

        to_char(sa.sbadata,\'dd/mm/YYYY HH24:MI:SS\') as data,

        coalesce(ss.ssudescricao, \'Não informado\') as ssudescricao,

        coalesce(fat.foadsc, \'Não informado\')      as fatendimento,

        coalesce(fex.frmdsc, \'Não informado\')      as frmexecucao

    FROM

        cte.subacaoindicador sa

    LEFT JOIN

        cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)

    LEFT JOIN

        cte.statussubacao ss on ss.ssuid = spt.ssuid

    LEFT JOIN

        cte.formaatendimento fat on fat.foaid = sa.foaid 

    LEFT JOIN

        cte.formaexecucao fex on fex.frmid = sa.frmid 

    LEFT JOIN

        seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)

    WHERE

        sa.sbaid = ' . (integer) $_REQUEST['titleFor'];
    
    if (($dados = $db->pegaLinha($sql)) !== false) {
    	
    	if($_REQUEST['titleFor']){
			$sql = 'SELECT sptano, 
	    	   			   coalesce(ss.ssudescricao, \'Não informado\') as ssudescricao FROM cte.subacaoparecertecnico spt
					LEFT JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
					WHERE spt.sbaid = ' . (integer) $_REQUEST['titleFor'] .' ORDER BY sptano ';
	    	$arStatus = $db->carregar($sql);
    	}
    	$arStatus = (is_array($arStatus)) ? $arStatus : array();
    	$stStatus = "";
    	foreach ($arStatus as $status){
    		$stStatus .= "&nbsp;&nbsp;&nbsp;".$status['sptano'] .': '. $status['ssudescricao']."<br />"; 
    	}
    	
    	$stStatus = ($stStatus) ? $stStatus : "&nbsp;&nbsp;&nbsp;Não Informado";
    	header("Content-Type: text/html; charset=ISO-8859-1");     	
        echo ''

            ,'Última atualização feita por '   , $dados['usunome']      , ' em ' , $dados['data'] , '<br /><br />'

//            ,'<strong>Status: </strong>'            , $dados['ssudescricao'] , '<br />'
            ,'<strong>Status: </strong>',  '<br />'
            ,$stStatus,  '<br />'

            ,'<strong>Cronograma: </strong>'        , $dados['sbaporescola'] , '<br />'

            ,'<strong>Forma de execução: </strong>' , $dados['frmexecucao'];

    } else {

        echo '<em>Nenhuma atualização realizada.</em>';

    }

	

    die();

}



	function exibeErrosSubAcao($erros) { 	

		echo " <script>

				function alterarSubacao( sbaid ){

					var janela = window.open( \"../brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=\" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

					janela.focus();

				}

		  	  </script>

		  

			  <div style=\"width : 100%;\">

				<table class=\"tabela\">

				<tr>

				<td colspan='2'>O sistema verificou que alguns dados das seguintes subações não foram preenchidos :</td>

				</tr>";

	

		foreach($erros as $idsubacao => $dados) {

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



# Validar 

# Deletar subacaoindicador



if ($_REQUEST['delete'] == 1 &&

    $_REQUEST['sbaid']       &&

    cte_podeEditarParecer($_SESSION['inuid']))

{

    $sql = '

    SELECT

        *

    FROM

        cte.subacaoparecertecnico spt

    WHERE

        sbaid = ' . $_REQUEST['sbaid'] . ' AND

        (

            ssuid IS NOT NULL or

            trim(sptparecer) <> \'\' or

            trim(sptuntdsc)  <> \'\'

        )';



    $parec = $db->pegaUm($sql);



    $sql = '

    SELECT

        COUNT(sac.sbaid)

    FROM

        cte.subacaoconvenio sac

    INNER JOIN

        cte.subacaoindicador sba ON sba.sbaid = sac.sbaid

    WHERE

        sac.sbaid = ' . $_REQUEST['sbaid'];



    $conv  = $db->pegaUm($sql);



    if ($parec >= 1 || $conv >= 1) {

        echo '<script type="text/javascript">'

            .'alert("Operação não realizada!\nFoi dado o parecer técnico ou foi gerado convênio.");'

            .'window.location.href = "?modulo=principal/par_subacao&acao=A&sbaid=' . $_REQUEST['sbaid'].'";'

            .'</script>';

    } else {

        $db->executar('

                       DELETE FROM

                        cte.subacaoparecertecnico

                       WHERE

                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';



                       DELETE FROM

                        cte.subacaobeneficiario

                       WHERE

                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';



                       DELETE FROM

                        cte.composicaosubacao

                       WHERE

                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';



                       DELETE FROM

                        cte.qtdfisicoano

                       WHERE

                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';



                       DELETE FROM

                        cte.termosubacaoindicador

                       WHERE

                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';



                       DELETE FROM

                        cte.subacaoindicador

                       WHERE

                        sbaid = ' . (integer) $_REQUEST['sbaid']);



        $db->commit();



        echo '<script>'

            ,'alert(\'Operação realizada com sucesso!\');'

            ,'location.href = \'?modulo=principal/estrutura_avaliacao&acao=A\''

            ,'</script>';

    }



    die();

}



//cte_atualizarPlanosPequenoMunicipio( $_SESSION['inuid'] );

//$db->commit();



include_once APPRAIZ . 'includes/workflow.php';

include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {

$db->cria_aba( $abacod_tela, $url, '' );

cte_montaTitulo(

$titulo_modulo, '

		<img src="/imagens/atencao.png" align="middle"/> Itens com pendências

			&nbsp;&nbsp;&nbsp;

			<img src="/imagens/check_p.gif" align="middle"/> Itens ok'

			);

} else {

$db->cria_aba( $abacod_tela, $url, '' );

cte_montaTitulo( $titulo_modulo, '' );

}

//print_r( $_SESSION);



$itrid = cte_pegarItrid( $_SESSION['inuid'] );
$_SESSION['brasilpro']['itrid'] = $itrid;


$sql_dimensao = sprintf(

	"select d.dimid, d.dimcod, d.dimdsc

	from cte.dimensao d

	where d.dimstatus = 'A' and d.itrid = %d

	order by d.dimcod",

$itrid

);

$dimensoes = $db->carregar( $sql_dimensao );

if ( !$dimensoes )

{

$dimensoes = array();

}



$sql_area = sprintf(

	"select a.ardid, a.ardcod, a.arddsc, a.dimid

	from cte.areadimensao a

	inner join cte.dimensao d on d.dimid = a.dimid

	where a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d

	order by a.ardcod",

$itrid

);



$areas = $db->carregar( $sql_area );

if ( !$areas ) {

$areas = array();

}



$sql_indicador = sprintf("

	SELECT DISTINCT

		i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, length(p.ptodemandaestadual) as demandaestadual, 

		length(p.ptodemandamunicipal) as demandamunicipal, 

		c.ctrpontuacao, aie.aciid as acaoestadual, 

		aim.aciid as acaomunicipal, saie.aciid as subacaoestadual, 

		saim.aciid as subacaomunicipal, 

		usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data 

	FROM cte.indicador i 

	INNER JOIN cte.areadimensao a on a.ardid = i.ardid 

	INNER JOIN cte.dimensao d on d.dimid = a.dimid 

	LEFT  JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A' 

	LEFT  JOIN cte.criterio c on c.crtid = p.crtid

	LEFT  JOIN cte.acaoindicador aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E' 

	LEFT  JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 

	LEFT  JOIN cte.subacaoindicador saie on saie.aciid = aie.aciid 

	LEFT  JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid 

	LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)

	WHERE

		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d 

	GROUP BY i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, p.ptodemandaestadual, 

		 p.ptodemandamunicipal, c.ctrpontuacao, saie.aciid, saim.aciid, aie.aciid, aim.aciid, usu.usunome, p.ptodata

	ORDER BY

		i.indcod",

$_SESSION['inuid'],

$itrid

);

$indicadores = $db->carregar( $sql_indicador );

if ( !$indicadores )

{

$indicadores = array();

}



$sql_dimensao_q = sprintf(

	"select d.dimid, d.dimcod, d.dimdsc

	from cte.dimensao d

	where d.dimstatus = 'A' and d.itrid = %d

	and exists ( select p.prgid from cte.pergunta p where p.dimid = d.dimid )

	order by d.dimcod",

$itrid

);

$dimensoes_q = $db->carregar( $sql_dimensao_q );

if ( !$dimensoes_q )

{

$dimensoes_q = array();

}

/*

 $sql_pergunta = sprintf(

 "select p.prgid, p.prgdsc, p.prgcod, d.dimid, r.rspid

 from cte.pergunta p

 inner join cte.dimensao d on d.dimid = p.dimid

 left join cte.resposta r on r.prgid = p.prgid and r.inuid = %d

 where p.prgstatus = 'A' and d.dimstatus = 'A'

 order by p.prgcod",

 $_SESSION['inuid']

 );

 */

$sql_pergunta = sprintf(

	"SELECT p.prgid, p.prgdsc, p.prgcod, d.dimid, r.rspid,

		usu.usunome, to_char( r.rspdata,'dd/mm/YYYY') as data 

	from cte.pergunta p

	inner join cte.dimensao d on d.dimid = p.dimid

	left join cte.resposta r on r.prgid = p.prgid and r.inuid = %d

	left JOIN seguranca.usuario AS usu ON (r.usucpf = usu.usucpf)

	where p.prgstatus = 'A' and d.dimstatus = 'A'

	order by p.prgcod",

$_SESSION['inuid']

);









$perguntas = $db->carregar( $sql_pergunta );

if ( !$perguntas )

{

$perguntas = array();

}



$sql_arquivo = sprintf(

	"select t.*, a.*

	from cte.tabelas t

	inner join public.arquivo a on a.arqid = t.arqid

	where inuid = %d

	order by a.arqnome",

$_SESSION['inuid']

);

$arquivos = $db->carregar( $sql_arquivo );

if ( !$arquivos )

{

$arquivos = array();

}



$sql_acao_estadual = sprintf(

	"SELECT 

		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, count( sai.sbaid ) as subacoes,

		usu.usunome, to_char( ai.acidata,'dd/mm/YYYY') as data 

	FROM 

		cte.indicador i

	INNER JOIN cte.areadimensao a on a.ardid = i.ardid

	INNER JOIN cte.dimensao d on d.dimid = a.dimid

	INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A'

	INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'E'

	LEFT  JOIN cte.subacaoindicador sai on sai.aciid = ai.aciid

		--descobre quem fez a ultima atualização

		LEFT JOIN seguranca.usuario AS usu ON (ai.usucpf = usu.usucpf)

	WHERE

		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d

	GROUP BY

		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, usu.usunome, ai.acidata",

$_SESSION['inuid'],

$itrid

);



$acoes_estaduais = $db->carregar( $sql_acao_estadual );

if ( !$acoes_estaduais )

{

$acoes_estaduais = array();

}



$sql_acao_municipal = sprintf(

	"SELECT 

		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, count( sai.sbaid ) as subacoes,

		usu.usunome, to_char( ai.acidata,'dd/mm/YYYY') as data 

	FROM 

		cte.indicador i

	INNER JOIN cte.areadimensao a on a.ardid = i.ardid

	INNER JOIN cte.dimensao d on d.dimid = a.dimid

	INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A'

	INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'M'

	LEFT  JOIN cte.subacaoindicador sai on sai.aciid = ai.aciid

		--descobre quem fez a ultima atualização

		LEFT JOIN seguranca.usuario AS usu ON (ai.usucpf = usu.usucpf)

	WHERE

		i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d

	GROUP BY

		ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid, usu.usunome, ai.acidata",

$_SESSION['inuid'],

$itrid

);

$acoes_municipais = $db->carregar( $sql_acao_municipal );

if ( !$acoes_municipais ) {

$acoes_municipais = array();

}

/*

$sql_subacao = sprintf("SELECT

				sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem,

				usu.usunome, to_char( sa.sbadata,'dd/mm/YYYY') as data,

				spt.ssuid, ss.ssudescricao

			FROM 

				cte.subacaoindicador sa 

			INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 

			INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = %d and p.ptostatus = 'A'

			LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid and spt.sptano = to_char(now(),'yyyy')

			LEFT JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid

			LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)

			order by sa.sbaordem, sa.sbadsc

",

$_SESSION['inuid'],

$itrid

);



*/



$sql_subacao = sprintf("SELECT

							DISTINCT

							CASE WHEN sa.sbaporescola <> false 

							THEN 'Por escola'

        					ELSE 'Global' 

        					END as sbaporescola,

        					sa.frmid,

        					sbaporescola,

							sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem,
							
							sa.sbaidpai,

							usu.usunome, to_char( sa.sbadata,'dd/mm/YYYY') as data,

							spt.ssuid, ss.ssudescricao,

							coalesce(fex.frmdsc, 'Nao informado') as frmexecucao--,
							
							--(SELECT DISTINCT
							--	prsnumconvsape
							--FROM cte.projetosape psi
							--INNER JOIN cte.projetosapesubacao pssi ON psi.prsid = pssi.prsid
							--WHERE pssi.sbaid = sa.sbaid) as prsnumconvs

						FROM 

							cte.subacaoindicador sa 

						INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 

						INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = %d and p.ptostatus = 'A'
						
						LEFT JOIN cte.projetosape ps ON ps.inuid = p.inuid 
						
						LEFT JOIN cte.projetosapesubacao pss ON ps.prsid = pss.prsid

						LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid and cast(spt.sptano as varchar) = to_char(now(),'yyyy')

						LEFT JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid

						LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)

						LEFT JOIN cte.formaexecucao fex on fex.frmid = sa.frmid 

						order by sa.sbaordem, sa.sbadsc

",


$_SESSION['inuid'],

$itrid

);

//dbg($sql_subacao,1);


/*

 * Backup do sql que estava anteriormente

$sql_subacao = sprintf("SELECT

				sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem,

				usu.usunome, to_char( sa.sbadata,'dd/mm/YYYY') as data,

				ss.ssuid, ss.ssudescricao

			FROM 

				cte.subacaoindicador sa 

			INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 

			INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = %d and p.ptostatus = 'A'

			--LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid and cast(spt.sptano as varchar) = to_char(now(),'yyyy')

			LEFT JOIN cte.statussubacao ss on ss.ssuid = sa.ssuid

			LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)

			order by sa.sbaordem, sa.sbadsc

",

$_SESSION['inuid'],

$itrid

);

*/

$subacoes = $db->carregar( $sql_subacao );

$sbaidsAditivos = array();
foreach($subacoes as $subacao){	
	
	if($subacao['sbaidpai'] != '' || $subacao['sbaidpai'] != null){
		$sbaidsAditivos[] = $subacao["sbaidpai"];		
	}
}

if ( !$subacoes ) {

	$subacoes = array();

}

$docid = cte_pegarDocid( $_SESSION['inuid'] );

$estado_documento = wf_pegarEstadoAtual( $docid );


// AÇÕES DO PLANO DE METAS



if ( cte_pegarItrid( $_SESSION['inuid'] ) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {

$_itrid = 1;

$condicao = " and iu.estuf = '". cte_pegarEstuf( $_SESSION['inuid'] ) ."' ";
$_SESSION['brasilpro']['estuf'] = cte_pegarEstuf( $_SESSION['inuid'] );
} else {

$_itrid = 2;

$condicao = " and iu.muncod = '". cte_pegarMuncod( $_SESSION['inuid'] ) ."' ";
$_SESSION['brasilpro']['muncod'] = cte_pegarMuncod( $_SESSION['inuid'] );
}
//dbg($subacoes,1);

$sql = sprintf(

	"select d.dimid, ad.ardid, i.indid, ai.aciid, sai.sbaid 

	from cte.dimensao d

	inner join cte.areadimensao ad on ad.dimid = d.dimid and ad.ardstatus = 'A'

	inner join cte.indicador i on i.ardid = ad.ardid and i.indstatus = 'A'

	inner join cte.pontuacao p on p.indid = i.indid and p.ptostatus = 'A'

	inner join cte.instrumentounidade iu on iu.inuid = p.inuid and iu.itrid = %d %s

	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid

	inner join cte.subacaoindicador sai on sai.aciid = ai.aciid and sai.foaid = %d

	where d.dimstatus = 'A'

	order by d.dimid, ad.ardid, i.indid, ai.aciid, sai.sbaid",

$_itrid,

$condicao,

ATENDIMENTO_BRASIL_PROFISSIONALIZADO

);



$filtro = array();

foreach( (array) $db->carregar( $sql ) as $registro ){

if ( !$registro ) {

continue;

}

foreach ( $registro as $campo=> $valor ) {

$filtro[$campo] = is_array( $filtro[$campo] ) ? $filtro[$campo] : array();

if ( !in_array( $valor, $filtro[$campo] ) ) {

array_push( $filtro[$campo], $valor );

}

}

}



if ( $filtro ) {

$sql_dimensao = sprintf(

		"select d.dimid, d.dimcod, d.dimdsc

		from cte.dimensao d

		where d.dimid in ( %s )

		order by d.dimcod",

implode( ",", $filtro['dimid'] )

);

$pm_dimensoes = $db->carregar( $sql_dimensao );

}

if ( !$pm_dimensoes )

{

$pm_dimensoes = array();

}



if ( $filtro ) {

$sql_area = sprintf(

		"select a.ardid, a.ardcod, a.arddsc, a.dimid

		from cte.areadimensao a

		where a.ardid in ( %s )

		order by a.ardcod",

implode( ",", $filtro['ardid'] )

);

$pm_areas = $db->carregar( $sql_area );

}

if ( !$pm_areas )

{

$pm_areas = array();

}



if ( $filtro ) {

$sql_indicador = sprintf(

		"select i.indid, i.inddsc, i.ardid, i.indcod

		from cte.indicador i

		where i.indid in ( %s )

		order by i.indid",

implode( ",", $filtro['indid'] )

);

$pm_indicadores = $db->carregar( $sql_indicador );

}

if ( !$pm_indicadores )

{

$pm_indicadores = array();

}



if ( $filtro ) {

$sql_acao = sprintf(

		"select a.aciid, p.indid, a.acidsc, a.acilocalizador

		from cte.acaoindicador a

		inner join cte.pontuacao p on p.ptoid = a.ptoid

		where a.aciid in ( %s )",

implode( ",", $filtro['aciid'] )

);

$pm_acoes = $db->carregar( $sql_acao );

}

if ( !$pm_acoes ) {

$pm_acoes = array();

}



if ( $filtro ) {

$sql_subacao = sprintf(

		"select s.sbaid, s.aciid, s.sbadsc, s.sbaordem

		from cte.subacaoindicador s

		where s.sbaid in ( %s )

		order by s.sbaordem, s.sbadsc",

implode( ",", $filtro['sbaid'] )

);

$pm_subacoes = $db->carregar( $sql_subacao );

}

if ( !$pm_subacoes ) {

$pm_subacoes = array();

}



function delimitador($texto){

	if(strlen($texto) > 120){

		$texto = substr($texto,0,120).'...';

	}

	return $texto;

}



?>

<script

	language="javascript" type="text/javascript"

	src="../includes/dtree/dtree.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">

<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>

<script type="text/javascript" src="../includes/remedial.js"></script>

<script type="text/javascript" src="../includes/superTitle.js"></script>

	

<table align="center" border="0" class="tabela" cellpadding="3"

	cellspacing="1">

	<colgroup>

		<col />

	</colgroup>

	<tbody>

		<tr>

			<td

				style="padding: 15px; background-color: #fafafa; color: #404040; vertical-align: top;"

				colspan="4">

			<div id="bloco" style="overflow: hidden;">

			<? 

			if($_REQUEST['exibirerros'] && $_SESSION['validaErro']) {

				if(!$_SESSION['validaErroAtualizado']) {

					echo "<script> 

							location.href = '/brasilpro/brasilpro.php?modulo=principal/enviar&acao=A';

		  				  </script>";

					exit;

				}

				exibeErrosSubAcao($_SESSION['validaErro']);

				$_SESSION['validaErroAtualizado'] = false;

			} else {	

			?>

			

			<p><a href="javascript: arvore.openAll();">Abrir Todos</a>

			&nbsp;|&nbsp; <a href="javascript: arvore.closeAll();">Fechar Todos</a>

			</p>

			<div id="_arvore"></div>

			<? } ?>

			</div>

			</td>

			<td style="background-color: #fafafa; color: #404040; width: 1px;"

				valign="top"><?php if( cte_pegarEstuf( $_SESSION['inuid'] ) == "RJ" ): ?>

			<table cellspacing="0" cellpadding="3" border="0"

				style="border: 2px solid rgb(201, 201, 201); background-color: rgb(245, 245, 245); width: 80px;">

				<tbody>

					<tr

						style="background-color: rgb(201, 201, 201); text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><span

							title="estado atual"> <b>Arquivos</b> </span></td>

					</tr>

					<tr style="text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><a href="#"

							onclick="var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/arquivo.htm', 'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes'); janela.focus();">Termo

						de Cooperação</a></td>

					</tr>

				</tbody>

			</table>

			<p>&nbsp;</p>

			<?php endif; ?> <?php wf_desenhaBarraNavegacao( $docid, array( 'inuid' => $_SESSION['inuid'] ) ); ?>







			<!-- AÇÕES -->

			

<!-- ------------------------------------------------------------------------------ -->

 <script>

			function verificaPendencia( ){

				var janela = window.open( "brasilpro.php?modulo=principal/verificarpendencias&acao=C", 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

				janela.focus();

			}

		  </script>

<?

if( ( $estado_documento["esdid"] == CTE_ESTADO_PAR ) ){

?>

	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

		<tr style="background-color: #c9c9c9; text-align:center;">

			<td style="font-size:7pt; text-align:center;">

				<span title="Cadastramento de parecer">

					<strong>Preenchimento</strong>

				</span>

			</td>

		</tr>

		<tr style="text-align:center;">

			<td style="font-size:7pt; text-align:center;">

              <a  title="Verificar pendencias" onclick="verificaPendencia();">Verificar pendências</a>

			</td>

		</tr>

	</table><br /><br />

<?php

}

if(($estado_documento["esdid"] == CTE_ESTADO_FIANCEIRA) && $possuiPerfil  ){

?>

 <table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom:20px;">

					<tr style="background-color: #c9c9c9; text-align:center;">

						<td style="font-size:7pt; text-align:center;">

							<span title="Cadastramento de parecer">

								<strong>Preenchimento</strong>

							</span>

						</td>

					</tr>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

						<script type="text/javascript">function abrePendencias(){window.open('brasilpro.php?modulo=principal/pendencia&acao=C','gera','width=600,height=750,scrollbars=1');}</script>

                         <a style="cursor: pointer" onclick="abrePendencias();">Verificar pendências</a>

						</td>

					</tr>

	</table><br />

    



<?

}





if ( defined('CTE_ESTADO_PARECER') && $estado_documento["esdid"] == CTE_ESTADO_PARECER )

{



    $sql = "

        select

            pe.pflcod,

            pe.pfldsc

        from

            seguranca.perfilusuario pu

        inner join

            seguranca.perfil pe

        on

            pu.pflcod = pe.pflcod

        where

            usucpf = '" . $_SESSION['usucpf'] . "' and

            pe.pfldsc like '%Parecerista %' OR pe.pfldsc like 'Super%'";



    $res = $db->carregar($sql);

    $perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PARECERISTA_FNDE);

    $possuiPerfil = cte_possuiPerfil($perfis);



    if (is_array($res)) {

        $fisico     = false;

        $engenharia = false;

        $juridico   = false;



        foreach ($res as $linha) {

            $fisico     = $fisico     || trim($linha['pfldsc']) == 'Parecerista Fisico/Financeiro';

            $engenharia = $engenharia || trim($linha['pfldsc']) == 'Parecerista da Engenharia';

            $juridico   = $juridico   || trim($linha['pfldsc']) == 'Parecerista Jurídico PAR';

        }

    }



	if ($possuiPerfil || $fisico || $engenharia || $juridico) {

?>



				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

					<tr style="background-color: #c9c9c9; text-align:center;">

						<td style="font-size:7pt; text-align:center;">

							<span title="Cadastramento de parecer">

								<strong>parecer</strong>

							</span>

						</td>

					</tr>

<?php



        if ($possuiPerfil || $fisico) {

?>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                          <a href="brasilpro.php?modulo=principal/cadastrarparecer&acao=A" title="Cadastramento de parecer Físico Financeiro">Físico Financeiro</a>

						</td>

					</tr>

<?php

        }

        if ($possuiPerfil || $engenharia) {

?>



					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                          <a href="brasilpro.php?modulo=principal/cadastrarparecer&acao=B" title="Cadastramento de parecer de Engenharia">Engenharia</a>

						</td>

					</tr>

<?php

        }                                                                

?>

				</table><br /><br />

<?php

    }

}





//dbg($_SESSION['inuid'],1);

	

				//gera convênio

				$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES);

				$possuiPerfil = cte_possuiPerfil($perfis);

				

				if ($possuiPerfil && $estado_documento["esdid"] == CTE_ESTADO_FNDE) {

				//if (!cte_convenioFNDEConcluido($_SESSION['inuid']) ){ ?>

				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

					<tr style="background-color: #c9c9c9; text-align:center;">

						<td style="font-size:7pt; text-align:center;">

							<span title="Cadastramento de parecer">

								<strong>FNDE</strong>

							</span>



                            <script type="text/javascript">

                            function enviarPar()

                            {

                               // window.location.href = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=principal/enviar&acao=A';

                               window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/brasilpro.php?modulo=principal/enviar&acao=A','Envio de projeto para o SAPE','width=1024,height=600,scrollbars=1');

                               return false;

                            }

                            </script>

						</td>

					</tr>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                      <a href="javascript:void(0);" onclick="return enviarPar();" title="Gerar Convênio no FNDE">Gerar Convênio</a>

						</td>

					</tr>

                </table><br />

			   <?php

	//}else

	//{

			$envia_docid = cte_pegarDocid( $_SESSION['inuid'] );

			$envia_documento = wf_pegarEstadoAtual( $envia_docid );

			$envia_esdid = (integer) $envia_documento['esdid'];

	$perfis = array( CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_ALTA_GESTAO, CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES );

	$possuiPerfil = cte_possuiPerfil( $perfis );

	?>

				<?php if ( $estado_documento["esdid"] == CTE_ESTADO_FNDE ){ ?>
					<table border="0" cellpadding="3" cellspacing="0"
						style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
						<tr style="background-color: #c9c9c9; text-align: center;">
							<td style="font-size: 7pt; text-align: center;"><span
								title="Cadastramento de parecer"><strong>PTA</strong></span> <script
								type="text/javascript">
							function visualizarPTA(){
								window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/brasilpro.php?modulo=principal/pta/visualizarPTA&acao=A','Visualizar PTA','width=500,height=300,scrollbars=1');
							}
						</script></td>
						</tr>
						<tr style="text-align: center;">
							<td style="font-size: 7pt; text-align: center;"><a
								href="javascript:void(0);" onclick="return visualizarPTA();"
								title="Visualizar PTA">Visualizar PTA</a></td>
						</tr>
					</table>
					<br />
				<?php } ?>

				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

					<tr style="background-color: #c9c9c9; text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                        <span title="Cadastramento de parecer"><strong>FNDE</strong></span>

                        <script type="text/javascript">function verRelatorioEnvioFnde(){window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/cte.php?modulo=principal/enviar_relatorio&acao=A','RelatoriodeConvenio','width=1024,height=600,scrollbars=1');}</script>

						</td>

					</tr>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                        <a style="cursor: pointer" onclick="verRelatorioEnvioFnde();">Relatório de Envio de Convênio</a>

						</td>

					</tr>

                </table><br />

		

<!-- ------------------------------------------------------------------------- -->

<?php

	}

	if ($possuiPerfil && $estado_documento["esdid"] == CTE_ESTADO_FINALIZADO) {

?>

	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

					<tr style="background-color: #c9c9c9; text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                        <span title="Cadastramento de parecer"><strong>FNDE</strong></span>

                        <script type="text/javascript">function verRelatorioEnvioFnde(){window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/brasilpro.php?modulo=principal/enviar_relatorio&acao=A','RelatoriodeConvenio','width=1024,height=600,scrollbars=1');}</script>

						</td>

					</tr>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

                        <a style="cursor: pointer" onclick="verRelatorioEnvioFnde();">Relatório de Envio de Convênio</a>

						</td>

					</tr>

                </table><br />



<?	

	}

	$sql = sprintf("SELECT

						esdordem

					FROM 

						workflow.documento d

						inner join workflow.estadodocumento ed on ed.esdid = d.esdid

					WHERE

						d.docid = %d;", $docid);

	

	$esdOrdem = $db->pegaUm($sql);



    if($db->testa_superuser() /*&& $esdOrdem > 3*/ ){

?>

			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom:20px;">

					<tr style="background-color: #c9c9c9; text-align:center;">

						<td style="font-size:7pt; text-align:center;">

							<span title="Cadastramento de parecer">

								<strong>Gerar</strong>

							</span>

						</td>

					</tr>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

						<script type="text/javascript">

						function abreGeracaoDeDocumentos(){

							window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/geratermoparecer.php','gera','width=50,height=50,scrollbars=0');

						}

						</script>

                         <a style="cursor: pointer" onclick="abreGeracaoDeDocumentos();">

									<!-- Termo de Cooperação e  -->Parecer

								</a>

						</td>

					</tr>

					<tr style="text-align:center;">

						<td style="font-size:7pt; text-align:center;">

						<script type="text/javascript">

						function abreGeracaoNotaTecnica(){

							window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/geranotatecnica.php','gera','width=800,height=800,scrollbars=0');

						}

						</script>

                         <a style="cursor: pointer" onclick="abreGeracaoNotaTecnica();">

									Nota Técnica

								</a>

						</td>

					</tr>

			</table>
			

<!-- Menu Gerencia Projetos-->

<?	
	}
	$sql = sprintf("SELECT
						esdordem
					FROM 
						workflow.documento d
						inner join workflow.estadodocumento ed on ed.esdid = d.esdid
					WHERE
						d.docid = %d;", $docid);
	
	$esdOrdem = $db->pegaUm($sql);

    if($db->testa_superuser() /*&& $esdOrdem > 3*/ ){
?>
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px; margin-bottom:20px;">
					<tr style="background-color: #c9c9c9; text-align:center;">
						<td style="font-size:7pt; text-align:center;">
							<span title="Cadastramento de parecer">
								<strong>Projetos</strong>
							</span>
						</td>
					</tr>
					<tr style="text-align:center;">
						<td style="font-size:7pt; text-align:center;">
						<script type="text/javascript">
						function abreGerenciaProjetos(){
							window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/brasilpro.php?modulo=principal/par_listaprojetos&acao=A','gera','width=700,height=250,scrollbars=1');
						}
						</script>
                         <a style="cursor: pointer" onclick="abreGerenciaProjetos();">
									<!-- Termo de Cooperação e  -->Gerênciar<br>Projetos
								</a>
						</td>
					</tr>
			</table>

<?

}





	$perfis       = array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR);

$possuiPerfil = cte_possuiPerfil($perfis);



if ($possuiPerfil && $estado_documento["esdid"] == CTE_ESTADO_FNDE) {

?>

<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

<tr style="background-color: #c9c9c9; text-align:center;">

	<td style="font-size:7pt; text-align:center;">

		<span title="Deletar convênio e números de processos."><strong>FNDE</strong></span>

                            <script type="text/javascript">function 

                            deletarNumProcessos(){

                          		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/deletarNumProcessos.php?acao=A','blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

                          		}

		</script>

	</td>

</tr>

<tr style="text-align:center;">

	<td style="font-size:7pt; text-align:center;">

                        <a href="javascript:void(0);" onclick="return deletarNumProcessos();" title="Deletar Convênio e números de processos.">Deletar Convênio e números de processos no SIMEC.</a>

	</td>

</tr>

                </table><br />



<?php

}









			if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){

				$sqlSituacao = "

								SELECT esdordem FROM cte.instrumentounidade itr

									INNER JOIN workflow.documento doc on itr.docid = doc.docid

									INNER JOIN workflow.estadodocumento esd on doc.esdid = esd.esdid

								WHERE inuid = ". $_SESSION['inuid'];

			}

			else{

				$muncod = cte_pegarMuncod( $_SESSION['inuid'] );

				$sqlSituacao = "

								SELECT coalesce(esd.esdordem, 99)

								FROM territorios.municipio mnu

									LEFT JOIN cte.instrumentounidade itr on mnu.muncod = itr.muncod and itr.itrid = 2

									LEFT JOIN workflow.documento doc on itr.docid = doc.docid

									LEFT JOIN workflow.estadodocumento esd on doc.esdid = esd.esdid AND esd.tpdid = 2 

								WHERE mnu.muncod = '". $muncod ."'

								order by esdordem

								limit 1;";

			}

			$ordem = $db->pegaUm( $sqlSituacao );

			

			if( $ordem != 1 ){ ?>

				<table border="0" cellpadding="3" cellspacing="0"

					style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

					<tr style="background-color: #c9c9c9; text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><span

							title="estado atual"> <b>ferramentas</b> </span></td>

					</tr>

					<tr style="text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><a

							style="cursor: pointer" onclick="return false;"> <img

							src="/imagens/print.png" alt="Versão para impressão"

							onclick="relatorioImpressao( 'A' )"> </a></td>

					</tr>

					<?php if ( cte_podeAnalisar( $_SESSION['inuid'] ) ) : ?>

					<tr style="text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><!-- <a style="cursor: pointer" onclick="relatorioImpressao2( 'A' )"> -->

						<a style="cursor: pointer"

							onclick="alert( 'Clique na subação que deseja analisar.' );">

						Analisar </a></td>

					</tr>

					<?php endif; ?>

					<?php if ( cte_podeVerRelatorioParCopia( $_SESSION['inuid'] ) ) : ?>

					<tr style="text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><a

							style="cursor: pointer" onclick="relatorioCompleto( 'C' );">

						Completo (Original) </a></td>

					</tr>

					<?php endif; ?>

					<?php if ( cte_podeVerRelatorioParAtual( $_SESSION['inuid'] ) ) : ?>

					<tr style="text-align: center;">

						<td style="font-size: 7pt; text-align: center;"><a

							style="cursor: pointer" onclick="relatorioCompleto( 'A' );">

						Completo (Atual) </a></td>

					</tr>

					<?php endif; ?>

					

						<?php

						

						$sql = sprintf("SELECT

											COUNT(*)

										FROM

											cte.parecer

										WHERE

											inuid = %d", $_SESSION[inuid]);

						$possuiParecer = $db->pegaUm($sql);
						
					

						if (($db->testa_superuser() || cte_possuiPerfil(143)) && $possuiParecer > 0 ) :

						?>

						<!-- 	

							<tr style="text-align:center;">			

								<td style="font-size:7pt; text-align:center;">

								<script type="text/javascript">

										

										function emitirTermo()

										{

											var janela = window.open(

												'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/documento/termo&acao=A',

												'termo',

												'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes'

											);

											janela.focus();

										}

										

									</script>

									<a style="cursor: pointer" onclick="emitirTermo();">

										Emitir<br/>Termo de Cooperação

									</a>

								</td>

							</tr>

						 -->	

							<tr style="text-align:center;">

								<td style="font-size:7pt; text-align:center;">

									<script type="text/javascript">

										

										function emitirParecer()

										{

											var janela = window.open(

												'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/documento/parecer&acao=A',

												'parecer',

												'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes'

											);

											janela.focus();

										}

										function emitirCooperacao()

										{

											var janela = window.open(

												'./documento/termocoptecnica.php',

												'parecer',

												'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes'

											);

											janela.focus();

										}

										function emitirCompromisso()

										{

											var janela = window.open(

												'./documento/termocompromisso.php',

												'compromisso',

												'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes'

											);

											janela.focus();

										}

										function emitirNotaTecnica(){

											var janela = window.open( 

												'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/documento/notatecnica&acao=A',

												'notaTecnica',

												'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

												janela.focus();

										}									

									</script>

									<a style="cursor: pointer" onclick="emitirParecer();">

										Emitir<br/>Parecer

									</a>

									<br />

									<br />

									<a style="cursor: pointer" onclick="emitirCooperacao();">

										Emitir Termo<br/>Cooperação

									</a>

									<br />

									<br />

									<a style="cursor: pointer" onclick="emitirCompromisso();">

										Emitir Termo<br/>Compromisso

									</a>

									<br />

									<br />

									<a style="cursor: pointer" onclick="emitirNotaTecnica();">

										Emitir Nota<br/>Técnica

									</a>
									<br />

								</td>

							</tr>

						<?php endif; ?>

					

				</table>

			<?php } ?>



			</td>

		</tr>



	</tbody>

</table>

<? if(!$_REQUEST['exibirerros']) { ?>

<script type="text/javascript">

/*

	Tudo que estava cte foi mudado para brasilpro



*/

	//impressao

	function relatorioImpressao( ptostatus ){

		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>

			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/impressao&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			janela.focus();

		<?php else : ?>

			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/impressao_mun&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			janela.focus();

		<?php endif; ?>

	}



	//impressao

	function relatorioImpressao2( ptostatus ){

		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>

			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/impressao2&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			janela.focus();

		<?php else : ?>

			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/impressao2_mun&acao=A&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			janela.focus();

		<?php endif; ?>

	}



	//impressao

	function relatorioCompleto( ptostatus ){

		<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) : ?>

			//var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/par_relatorio&acao=R&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/par_relatorio&acao=R&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			janela.focus();

		<?php else : ?>

			//var janela = window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/cte.php?modulo=relatorio/par_relatorio_mun&acao=R&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			var janela = window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/brasilpro/brasilpro.php?modulo=relatorio/par_relatorio_mun&acao=R&ptostatus=' + ptostatus,'blank','height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');

			janela.focus();

		<?php endif; ?>

	}

	

	//impressao

	function responder( dimensao, pergunta ){

		window.location = '?modulo=principal/questoes/questoespontuais&acao=C&dimid='+ dimensao +'&prgid='+ pergunta;

	}



	function avaliar( indicador ){

		window.location = '?modulo=principal/formulario_avaliacao&acao=A&indid=' + indicador;

	}



	function download( arquivo ){

		window.location = '../mostra_arquivo.php?id=' + arquivo;

	}



	// ação

	function cadastrarAcao( ptoid, acilocalizador ){

		window.location = "?modulo=principal/par_acao&acao=A&ptoid=" + ptoid + "&acilocalizador=" + acilocalizador;

	}



	function alterarAcao( aciid ){

		window.location = "?modulo=principal/par_acao&acao=A&aciid=" + aciid;

	}



	function excluirAcao( aciid ){

		if ( confirm( "Deseja excluir a ação?" ) ) {

			window.location = "?modulo=principal/par_acao&acao=A&aciid="+ aciid +"&action=remov&origem=<?= urlencode( $_SERVER['QUERY_STRING'] ) ?>";

		}

	}



	// subação

	function cadastrarSubacao( aciid )

    {

<?php

$docid              = cte_pegarDocid($_SESSION['inuid']);

$estadoDocumento    = wf_pegarEstadoAtual($docid);

if (!in_array($estadoDocumento['esdid'], array(30,31,26))) {

?>

	    var inuid = <?=$_SESSION['inuid'] ?>;

		return windowOpen( '?modulo=principal/par_subacao&acao=A&aciid=' + aciid + '&inuid=' + inuid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

<?php



} else {

    echo "alert('O cadastro de SubAções não é permitido para documentos em fase de análise financeira.');";

}



?>



	}



	function alterarSubacao( sbaid ){

		var janela = window.open( "?modulo=principal/par_subacao&acao=A&sbaid=" + sbaid, 'alterarSubacao', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

		janela.focus();

	}



	function alterarSubacaoPlanoDeMetas( sbaid ){

		var janela = window.open( "?modulo=principal/par_subacao_compartilhada&acao=A&sbaid=" + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

		janela.focus();

	}



	function excluirSubacao(sbaid)

    {

		if (confirm("Deseja excluir a subação?")) {

            if (this._windows && this._windows.length > 0) {

                for (var i = 0; i < this._windows.length; i++) {

                    if (typeof(this._windows[i].close) == 'function')

                        this._windows[i].close();

                }

            }



			window.location.href = window.location.href.replace('#','') + "&delete=1&sbaid=" + sbaid;

		}

	}

	

	function criarAditivoSubacao(sbaidpai, sbaid) {

        if (!sbaid) {

            if (confirm("Deseja criar um aditivo para esta subação?")) {

                return windowOpen( "?modulo=principal/aditivo_parsubacao&acao=A&pai_sbaid=" + sbaidpai, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

            }

		} else {

                return windowOpen( "?modulo=principal/aditivo_parsubacao&acao=A&pai_sbaid=" + sbaidpai + '&sbaid=' + sbaid, 'blank', 'height=600,width=900,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

        }

	}



	// dados de referencia

	function cadastrarDadosReferencia(){

		//inuid está na sessao

		window.location = "brasilpro.php?modulo=principal/dados_unidade&acao=A";

	}



	function dadosDemograficosUF( estuf )

	{

		var janela = window.open( 'http://portal.mec.gov.br/ide/2008/gerarTabela.php?estado=' + estuf + '&submit=Gerar+Dados', 'blank', 'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

		janela.focus();

	}

	function dadosDemograficosMun( estuf, muncod )

	{

		var janela = window.open( 'http://portal.mec.gov.br/ide/2008/gerarTabela.php?municipio=' + muncod + '&submit=Gerar+Dados', 'blank', 'height=600,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );

		janela.focus();

	}



	function gerenciarEscolas()

	{

		window.location.href = '?modulo=principal/escolas&acao=A';

	}

	

	function gerenciarEscolasProep()

	{

		window.location.href = '?modulo=principal/escolasproep&acao=A';

	}



    function cadastrarEquipeLocal()

    {

		return windowOpen('?modulo=principal/cadastrardadounidade&acao=A&opt=equipeLocal', 'cadastroDadosUnidade','height=650,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

    }



    function cadastrarSecretario()

    {

		return windowOpen('?modulo=principal/cadastrardadounidade&acao=A&opt=secretario', 'cadastroDadosUnidade','height=650,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

    }



    function cadastrarComiteLocal()

    {

		return windowOpen('?modulo=principal/cadastrardadounidade&acao=A&opt=comiteLocal', 'cadastroDadosUnidade','height=650,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

    }





    var u='/brasilpro/brasilpro.php?modulo=principal/estrutura_avaliacao&acao=A&titleFor=';



	arvore = new dTree( "arvore" );



	arvore.config.folderLinks = true;

	arvore.config.useIcons = true;

	arvore.config.useCookies = true; 



	<?php

	$sql = sprintf( "select itrdsc from cte.instrumento where itrid = %d", $itrid );

	$instrumento = $db->pegaUm( $sql );

	?>



	arvore.add( 0, -1, '<?= $instrumento ?>' );



	arvore.add(1, 0, '<b>Dados da Unidade</b>' , 'brasilpro.php?modulo=principal/dados_unidade&acao=A' );



    <?php

    /*!@

     * @cat Deixa esse comentário aqui enquanto eu termino de ajeitar :D

     * @author Douglas

     */

    if (preg_match('/localhost/', $_SERVER['SERVER_NAME'])) {

        echo "arvore.add('dadosUnidade_0', 1, 'Equipe Local' , 'javascript:cadastrarEquipeLocal()');\n"

            ."arvore.add('dadosUnidade_1', 1, 'Secretário(a) de Educação' , 'javascript:cadastrarSecretario()' );\n"

            ."arvore.add('dadosUnidade_2', 1, 'Comitê Local' ,              'javascript:cadastrarComiteLocal()' );\n";

    }

    //                                                                      */



    if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {

		echo "arvore.add( 2, 0, '<b>Dados Demográficos e Educacionais Quantitativos</b>', 'javascript:dadosDemograficosUF(\'" , cte_pegarEstuf($_SESSION['inuid']) , "\' )', '', '', '../includes/dtree/img/imgfolder.gif', '../includes/dtree/img/imgfolder.gif' );\n";

	} else {

        $munid = cte_pegarMuncod($_SESSION['inuid']);

        $estuf = $db->pegaUm("select estuf from territorios.municipio where muncod ='" . $munid . "'");

        echo "arvore.add( 2, 0, '<b>Dados Demográficos e Educacionais Quantitativos</b>', 'javascript:dadosDemograficosMun(\'" , $estuf , "\', \'" , $munid , "\' )', '', '', '../includes/dtree/img/imgfolder.gif', '../includes/dtree/img/imgfolder.gif' );\n";

    }



    echo "arvore.add( 3, 0, '<b>Escolas Atendidas</b>', 'javascript:gerenciarEscolas();' );\n";

    

    echo "arvore.add( 2, 0, '<b>Escolas PROEP</b>', 'javascript:gerenciarEscolasProep();' );\n";





	if ( cte_podeVerQuestaoPontual( $_SESSION["inuid"] ) ): ?>

		//arvore.add( 4, 0, '<b>Questões Pontuais</b>', '', '', '', '../includes/dtree/img/question.gif', '../includes/dtree/img/question.gif' );

		<?php foreach( $dimensoes_q as $dimensao ): 

		$dimensao['dimdsc'] = delimitador($dimensao['dimdsc']);

		?>

			<?php $texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; ?>

			arvore.add( 'dq_<?= $dimensao['dimid'] ?>', 4, '<?= $texto ?>' );

		<?php endforeach; 

		      foreach( $perguntas as $pergunta ): 



				$icone= $pergunta['rspid'] ? '../imagens/check_p.gif' : '';

				$cor = $icone ? '#959595' : '#133368';

				

				$title = "Última Atualização feita por ".$pergunta['usunome']." em ".$pergunta['data'];

				if(strlen(trim($pergunta['usunome']))<2)

					$title = "Nenhuma atualização realizada.";					

					

				$texto = '<span title="'.$title.'" style="color: '. $cor .';">'. $pergunta['prgcod'] .'. '. $pergunta['prgdsc'] .'</span>';

			?>

			arvore.add( 'i_<?= $pergunta['prgid'] ?>', 'dq_<?= $pergunta['dimid'] ?>', '<?= $texto ?>', 'javascript: responder(<?= $pergunta['dimid'] ?>,<?= $pergunta['prgid'] ?>); ', '', '', '<?= $icone ?>' );

		<?php endforeach; 

		endif; 

	

	     if ( cte_podeVerIndicador( $_SESSION["inuid"] ) ): ?>		

		arvore.add( 5, 0, '<b>Indicadores Qualitativos - Brasil Profissionalizado</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );

		<?php foreach( $dimensoes as $dimensao ): ?>

			<?php $texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; ?>

			arvore.add( 'd_<?= $dimensao['dimid'] ?>', 5, '<?= $texto ?>' );

		<?php endforeach; ?>

		

		<?php foreach( $areas as $area ): 

		$area['arddsc'] = delimitador($area['arddsc']);

		?>

			<?php $texto = '<span style="color: #000000;">'. $area['ardcod'] .'. '. $area['arddsc'] .'</span>'; ?>

			arvore.add( 'a_<?= $area['ardid'] ?>', 'd_<?= $area['dimid'] ?>', '<?= $texto ?>' );

		<?php endforeach; 		

		?>

		

		<?php foreach( $indicadores as $indicador ): 

		 $indicador['inddsc'] = delimitador($indicador['inddsc']);

		?>

			<?php

				$icone = '';

				$cor = '';

				$mensagens = array();

				if ( $indicador['ptoid'] ) {

					$icone = '../imagens/check_p.gif';

					$cor = '#959595';

				}

				

				if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ) {

					if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {

						if ( !$indicador['subacaoestadual'] && in_array( $indicador['ctrpontuacao'], array(1,2) ) ) {

							array_push( $mensagens, "A pontuação está baixa." );

						}

						if ( !$indicador['subacaoestadual'] && $indicador['demandaestadual'] > 0 ) {

							array_push( $mensagens, "Há demanda estadual." );

						}

						if ( !$indicador['subacaomunicipal'] && $indicador['demandamunicipal'] > 0 ) {

							array_push( $mensagens, "Há demanda municipal." );

						}

						if ( count( $mensagens ) > 0 ) {

							array_push( $mensagens, "É necessário descrever um plano de ação." );

							$icone = '../imagens/atencao.png';

							$cor = '#202020';

						}

					}

				}

				

				if ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ) {

					if( $estado_documento['esdid'] != CTE_ESTADO_DIAGNOSTICO ) {

						if ( !$indicador['subacaomunicipal'] && in_array( $indicador['ctrpontuacao'], array(1,2) ) ) {

							array_push( $mensagens, "A pontuação está baixa. É necessário descrever um plano de ação." );

						}

						if ( $indicador['acaomunicipal'] && in_array( $indicador['ctrpontuacao'], array(0,3,4) ) ) {

							array_push( $mensagens, "A pontuação não permite que exista ação para este indicador." );

						}

						if ( count( $mensagens ) > 0 ) {

							$icone = '../imagens/atencao.png';

							$cor = '#202020';

						}

					}

				}

				

				//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTE ÍTEM.

				$title = "Última Atualização feita por ".$indicador['usunome']." em ".$indicador['data'];

				if(strlen(trim($indicador['usunome']))<2)

					$title = "Nenhuma atualização realizada.";	

				

				array_push( $mensagens, $title) ;

				$texto = '<span style="color: '. $cor .';" title="'. implode(' - ', $mensagens ) .'">'. $indicador['indcod'] .'. '. $indicador['inddsc'] .'</span>';

						

				?>

					

			arvore.add( 'i_<?= $indicador['indid'] ?>', 'a_<?= $indicador['ardid'] ?>', '<?= $texto ?>', 'javascript: avaliar(<?= $indicador['indid'] ?>);', '', '', '<?= $icone ?>', '<?= $icone ?>' );

			

			<?php if( cte_podeElaborarPlanoDeAcoes( $_SESSION['inuid'] ) ): ?>

				

				<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ): ?>

					<?php if ( $indicador['demandaestadual'] > 0 || in_array( $indicador['ctrpontuacao'], array(1,2) ) ): ?>

						<?php

						$evento_plano = "";

						$icone_plano = "";

						if ( !$indicador['acaoestadual'] ) {

							$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'E\');";

							$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';

						}

						?>

						arvore.add( 'ipe_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano de Ação da Rede Estadual', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

					<?php endif; ?>

					<?php if ( $indicador['demandamunicipal'] > 0 ): ?>

						<?php

						$evento_plano = "";

						$icone_plano = "";

						if ( !$indicador['acaomunicipal'] ) {

							$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'M\');";

							$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';

						}

						?>

						arvore.add( 'ipm_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano de Ação das Redes Municipais', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

					<?php endif; ?>

				<?php endif; ?>

				

				<?php if ( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ): ?>

					<?php if ( in_array( $indicador['ctrpontuacao'], array(1,2) ) || $indicador['acaomunicipal'] ): ?>

						<?php

                      	   // verifica se é município e pequeno município

                      	   $pequenoMunicipio =

                      	   cte_pegarMuncod( $_SESSION['inuid'] ) &&

                      	   !cte_verificaGrandeMunicipio( $_SESSION['inuid'] );

						   $equipeTecnicaMec = cte_possuiPerfil(

                              array( CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES )

                           );

                           $superUser = $db->testa_superuser();

						?>

						<?php

						if (

							!$pequenoMunicipio || // pode quando não município OU

							(

								$pequenoMunicipio &&

								( $equipeTecnicaMec || $superUser )

							)

						):

						?>

							<?php

							$evento_plano = "";

							$icone_plano = "";

							if ( !$indicador['acaoestadual'] ) {

								$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'M\');";

								$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';

							}

							?>

							arvore.add( 'ipm_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano do Município', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

						<?php endif; ?>

						<?php

						$evento_plano = "";

						$icone_plano = "";

						if ( !$indicador['acaomunicipal'] ) {

							$evento_plano = "javascript:cadastrarAcao(". $indicador['ptoid'] .",\'M\');";

							$icone_plano = '<img align="absmiddle" src="/imagens/gif_inclui.gif"/> ';

						}

						?>

						arvore.add( 'ipm_<?= $indicador['indid'] ?>', 'i_<?= $indicador['indid'] ?>', '<?= $icone_plano ?>Plano de Ação do Município', '<?= $evento_plano ?>', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

					<?php endif; ?>

				<?php endif; ?>

				

			<?php endif; ?>

			

		<?php endforeach; ?>

		

		<?php foreach( $acoes_estaduais as $acao ): 

		$acao['acidsc'] = delimitador($acao['acidsc']);

		?>

			<?php

				$prefixo = '';

//				$prefixo = '<img align="absmiddle" onclick="javascript:alterarAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/alterar.gif" title="alterar ação"/> ';



                /*

				if ( $acao['subacoes'] == 0 ) {

					$prefixo .= '<img align="absmiddle" onclick="javascript:excluirAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir ação"/> ';

				}

                //                                                          */



				//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTA AÇÃO

				$title = "Última Atualização feita por ".$acao['usunome']." em ".$acao['data'];

				if(strlen(trim($acao['usunome']))<2)

					$title = "Nenhuma atualização realizada.";

				$texto = '<a href="javascript:alterarAcao('. $acao['aciid'] .');"  title="'. $title .'">'. str_replace( "'", "", str_replace( array( "\n", "\r", chr(10), chr(13) ), " ", trim( $acao['acidsc'] ) ) ) .'</a>';

			?>

			arvore.add( 'ai_<?= $acao['aciid'] ?>', 'ipe_<?= $acao['indid'] ?>', '<?= $prefixo ?><?= $texto ?>', 'javascript:void(0);', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );

			arvore.add( 'ais_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="cadastrar subação"/> Subações (<?=  $acao['subacoes'] ?>)', 'javascript:cadastrarSubacao(<?= $acao['aciid'] ?>);', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

		<?php endforeach; ?>



		<?php foreach( $acoes_municipais as $acao ): 

		$acao['acidsc'] = delimitador($acao['acidsc']);

		?>

			<?php

				$prefixo = '';

//				$prefixo = '<img align="absmiddle" onclick="javascript:alterarAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/alterar.gif" title="alterar ação"/> ';

                /*

				if ( $acao['subacoes'] == 0 && ( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL || cte_verificaGrandeMunicipio( $_SESSION['inuid'] ) ) ) {

					$prefixo .= '<img align="absmiddle" onclick="javascript:excluirAcao('. $acao['aciid'] .');" style="cursor: pointer;" src="/imagens/excluir.gif" title="excluir ação"/> ';

				}

				//                                                            */



				//DEFININDO QUEM FEZ A ULTIMA ALTERAÇÃO NESTA AÇÃO

				$title = "Última Atualização feita por ".$acao['usunome']." em ".$acao['data'];

				if(strlen(trim($acao['usunome']))<2)

					$title = "Nenhuma atualização realizada.";	

				$texto = '<a href="javascript:alterarAcao('. $acao['aciid'] .');" title="'. $title .'">'. str_replace( "'", "", str_replace( array( "\n", "\r", chr(10), chr(13) ), " ", trim( $acao['acidsc'] ) ) ) .'</a>';

			?>

			arvore.add( 'ai_<?= $acao['aciid'] ?>', 'ipm_<?= $acao['indid'] ?>', '<?= $prefixo ?><?= $texto ?>', 'javascript:void(0);', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );

			<?php

				$verificacao = ( $estado_documento['esdid'] == CTE_ESTADO_ANALISE ) && ( cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_EQUIPE_TECNICA, CTE_PERFIL_CONSULTORES ) ) );

			?>

			<?php if ( cte_verificaGrandeMunicipio( $_SESSION['inuid'] ) || $verificacao ): ?>

				arvore.add( 'ais_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="cadastrar subação"/> Subações (<?=  $acao['subacoes'] ?>)', 'javascript:cadastrarSubacao(<?= $acao['aciid'] ?>);', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

			<?php else: ?>

				arvore.add( 'ais_<?= $acao['aciid'] ?>', 'ai_<?= $acao['aciid'] ?>', 'Subações (<?=  $acao['subacoes'] ?>)', '', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );

			<?php endif; ?>

		<?php endforeach; ?>

		

		<?php foreach( $subacoes as $subacao ): 

			$subacao['sbadsc'] = delimitador($subacao['sbadsc']);	

			if ( $estado_documento['esdid'] == CTE_ESTADO_PAR ) {

            	$icone = "";

			} else {

				$subacao['ssudescricao'] = $subacao['ssudescricao'] ? $subacao['ssudescricao'] : "Não analisado";

				if( brp_validarSubAcaoFaseAnalise( $subacao ) ){

					$icone = '<img src="../imagens/check_p.gif" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';

				} else {

					$icone = '<img src="../imagens/atencao.png" title="'. $subacao['ssudescricao'] .'"/>&nbsp;';

				}
			}

			$perfil = array(CTE_PERFIL_SUPER_USUARIO,
							CTE_PERFIL_EQUIPE_TECNICA,
							CTE_PERFIL_CONSULTORES,
							CTE_PERFIL_EQUIPE_LOCAL,
							CTE_PERFIL_CONSULTORES,
							CTE_PERFIL_ALTA_GESTAO,
							CTE_PERFIL_EQUIPE_MUNICIPAL);

			$prefixo = '';
				
			$sql_conveniada = ("SELECT DISTINCT
									prsnumconvsape
								FROM cte.projetosape psi
								INNER JOIN cte.projetosapesubacao pssi ON psi.prsid = pssi.prsid
								WHERE pssi.sbaid = {$subacao["sbaid"]}"); 
				
			$subacao['prsnumconvs'] = $db->carregar($sql_conveniada);

			$iconeConvenio = "";
			
			$sql = "SELECT 
						pssano as sbcano 
					FROM cte.projetosapesubacao 
					WHERE 
						sbaid = {$subacao["sbaid"]}";
			
			$stAnos = $db->carregar($sql);

			if( is_array( $stAnos ) ){
                	
				$iconeConvenio = '<img align="absmiddle" src="/imagens/projeto.gif" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados: '.$stAnos.'."/>';
                	
				if ( is_array( $subacao['prsnumconvs'] ) ){
					$iconeConvenio = '<img align="absmiddle" src="/imagens/conveniada.png" alt="Anos conveniados: '.$stAnos.'." title="Anos conveniados:'.$stAnos.'."/>';
				} 
                	
//              if( brp_possuiAditivo( $subacao["sbaid"] ) ){
				if(in_array($subacao["sbaid"], $sbaidsAditivos)){
					$iconeConvenio .= '<img align="absmiddle" src="/imagens/aditivo.png" alt="Sub-ação possui Aditivo" title="Sub-ação possui Aditivo"/>';
				}	
			}

				



				$ordem = $subacao['sbaordem'] ? $subacao['sbaordem']."&nbsp;" : "";

				$texto = $icone . $iconeConvenio . $ordem . 

				'<a onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ", $subacao['sbadsc'] ).'</a>';

			?>

			arvore.add( 'sa_<?= $subacao['sbaid'] ?>', 'ais_<?= $subacao['aciid'] ?>', '<?= $prefixo.$texto  ?>', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );

		<?php



            endforeach; ?>

	<?php endif; ?>



	arvore.add( 'planodemetas', 0, '<b>Indicadores Qualitativos - Plano de Metas</b>', '', '', '', '../includes/dtree/img/globe.gif', '../includes/dtree/img/globe.gif' );

	<?php foreach( $pm_dimensoes as $dimensao ): 

		$dimensao['dimdsc'] = delimitador($dimensao['dimdsc']);

	?>

		<?php $texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; ?>

		arvore.add( 'pmd_<?= $dimensao['dimid'] ?>', 'planodemetas', '<?= $texto ?>' );

	<?php endforeach; ?>

	<?php foreach( $pm_areas as $area ): 

	$area['arddsc'] = delimitador($area['arddsc']);

	?>

		<?php $texto = '<span style="color: #000000;">'. $area['ardcod'] .'. '. $area['arddsc'] .'</span>'; ?>

		arvore.add( 'pma_<?= $area['ardid'] ?>', 'pmd_<?= $area['dimid'] ?>', '<?= $texto ?>' );

	<?php endforeach; ?>

	<?php foreach( $pm_indicadores as $indicador ): 

	$indicador['inddsc'] = delimitador($indicador['inddsc']);

	?>

		<?php $texto = '<span style="color: #000000;">'. $indicador['indcod'] .'. '. $indicador['inddsc'] .'</span>'; ?>

		arvore.add( 'pmi_<?= $indicador['indid'] ?>', 'pma_<?= $indicador['ardid'] ?>', '<?= $texto ?>', '', '', '', '../imagens/check_p.gif', '../imagens/check_p.gif' );

	<?php endforeach; ?>

	<?php foreach( $pm_acoes as $acao ): 

	$acao['acidsc'] = delimitador($acao['acidsc']);

	?>

		<?php $texto = '<span style="color: #000000;">'. $acao['acidsc'] .'</span>'; ?>

		<?php $texto_plano = $acao['acilocalizador'] == 'M' ? 'Plano de Ação das Redes Municipais' : 'Plano de Ação da Rede Estadual'; ?>

		arvore.add( 'pmail_<?= $acao['aciid'] ?>', 'pmi_<?= $acao['indid'] ?>', '<?= $texto_plano ?>' );

		arvore.add( 'pmai_<?= $acao['aciid'] ?>', 'pmail_<?= $acao['aciid'] ?>', '<?= $texto ?>', '', '', '', '../includes/dtree/img/page.gif', '../includes/dtree/img/page.gif' );

	<?php endforeach; ?>

	<?php foreach( $pm_subacoes as $subacao ): 

	$subacao['sbadsc'] = delimitador($subacao['sbadsc']);

	?>

		<?php

		if( cte_podeElaborarPlanoDeAcoes( $_SESSION['inuid'] ) ) {

			$acao = 'javascript:alterarSubacaoPlanoDeMetas('. $subacao['sbaid'] .');';

		}

		$ordem = $subacao['sbaordem'] ? "<b>" . $subacao['sbaordem'] . "</b>&nbsp;" : "";

		$texto = '<span style="color: #000000;">'. $ordem . $subacao['sbadsc'] .'</span>';

		?>

		arvore.add( 'pmsai_<?= $subacao['sbaid'] ?>', 'pmai_<?= $subacao['aciid'] ?>', '<?= str_replace(array("\r\n", "\n", "\r"), ' ', $texto) ?>', '<?= $acao ?>' );

	<?php endforeach; ?>

	

	elemento = document.getElementById( '_arvore' );

	elemento.innerHTML = arvore;

	

	/*

	 * adaptação de largura e controle de overflow de conteúdo para o internet explorer

	 */

	var dav = navigator.appVersion;

	IE = document.all ? true : false;

	IE6 = dav.indexOf( "MSIE 6.0" ) >= 0;

	IE7 = dav.indexOf( "MSIE 7.0" ) >= 0;

	if ( IE ) {

		width = document.body.offsetWidth;

		height = document.body.offsetHeight;

		document.getElementById( 'bloco' ).style.width = ( width * 0.80 ) + 'px';

	}

	

</script>

<? } ?>





