<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>

<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?
ini_set("memory_limit", "1024M");
$filtrourlm = array();
$arrUrlInterna = array();

if($_REQUEST['estuf']) {
	$filtro[] = "ed.estuf='".$_REQUEST['estuf']."'";
	$filtro2[] = "ed.estuf='".$_REQUEST['estuf']."'";
	$arrUrlInterna[] = "&estuf=".$_REQUEST['estuf'];
	$filtrourlm[] = "&estuf=".$_REQUEST['estuf'];
	$titulo = "Estado: ".$db->pegaUm("SELECT estdescricao FROM territorios.estado WHERE estuf='".$_REQUEST['estuf']."'");
}
if($_REQUEST['muncod']) {
	$filtro[] = "ed.muncod='".$_REQUEST['muncod']."'";
	$filtro2[] = "ed.muncod='".$_REQUEST['muncod']."'";
	$arrUrlInterna[] = "&muncod=".$_REQUEST['muncod'];
	$filtrourlm[] = "&muncod=".$_REQUEST['muncod'];
	$titulo = "Município: ".$db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
}
if($_REQUEST['unicod']) {
	$filtro[] = "ent.entunicod='".$_REQUEST['unicod']."'";
	$filtro2[] = "ent.entunicod='".$_REQUEST['unicod']."'";
	$arrUrlInterna[] = "&unicod=".$_REQUEST['unicod'];
	$filtrourlm[] = "&unicod=".$_REQUEST['unicod'];
	$titulo = "Unidade: ".$db->pegaUm("SELECT unidsc FROM public.unidade WHERE unicod='".$_REQUEST['unicod']."'");
}
if($_REQUEST['entid']) {
	$filtro[] = "ent2.entid='".$_REQUEST['entid']."'";
	$filtro2[] = "ent.entid='".$_REQUEST['entid']."'";
	$arrUrlInterna[] = "&entid=".$_REQUEST['entid'];
	$filtrourlm[] = "&entid=".$_REQUEST['entid'];
	$titulo = "Campus: ".$db->pegaUm("SELECT entnome FROM entidade.entidade WHERE entid='".$_REQUEST['entid']."'");
}

if($_REQUEST['stoid']) {
	$filtrourlm[] = "&stoid=".$_REQUEST['stoid'];
	$titulo2 = ", ".$db->pegaUm("SELECT DISTINCT situacao
FROM (
	SELECT stodesc AS situacao FROM obras.situacaoobra WHERE stoid IN (".$_REQUEST['stoid'].")
UNION ALL
	SELECT esddsc AS situacao FROM workflow.estadodocumento WHERE esdid IN (".$_REQUEST['stoid'].")
) AS FOO");
}


/* montando o menu */
// monta menu padrão contendo informações sobre as entidades
$menu = array(
			  0 => array("id" => 0, "descricao" => "Todos",     			"link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A".(($_REQUEST['orgid'])?"&orgid=todos":"").implode("",$arrUrlInterna)),
      		  1 => array("id" => 1, "descricao" => "Educação Superior",     "link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&orgid=1".implode("",$arrUrlInterna)),
			  2 => array("id" => 2, "descricao" => "Educação Profissional", "link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&orgid=2".implode("",$arrUrlInterna)),
			  3 => array("id" => 3, "descricao" => "Educação Básica", 		"link" => "/painel/painel.php?modulo=relatorio/relatorioObra&acao=A&orgid=3".implode("",$arrUrlInterna)),
		  	  );

if(!$_REQUEST['orgid']) $_REQUEST['orgid']="todos";
if(!$titulo) $titulo="País: Brasil";

if($_REQUEST['orgid']) {
	if($_REQUEST['orgid'] != "todos"){
		$filtro[] = "oi.orgid='".$_REQUEST['orgid']."'";
		$filtro2[] = "e.orgid='".$_REQUEST['orgid']."'";
    }
	array_unshift($arrUrlInterna, "&orgid=".$_REQUEST['orgid']);
}

if(!$_REQUEST['pagina']) {
	$_REQUEST['pagina']=0;
}

if($_REQUEST['stoid']) {
    $filtro[] = "so.stoid IN (".$_REQUEST['stoid'].")";
    $filtro2[] = "d.esdid IN (".$_REQUEST['stoid'].")";
    array_unshift($arrUrlInterna, "&stoid=".$_REQUEST['stoid']);
}

echo "<br>"; 
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
monta_titulo( "Relatório de obras (".$titulo.$titulo2.") ", "<b><a href='/painel/painel.php?modulo=relatorio/relatorioObra&acao=A".implode("",$arrUrlInterna)."'>Listar todas as obras</a></b>");
?>
<table class="tabela" cellSpacing="1" cellPadding="3" align="center" style="border:1px solid #ccc; background-color:#fff;">
	<tr>
 		<td>
            <table cellspacing="1" cellpadding="3" width="100%">
                <tr>
                    <td style="text-align: center; background-color: #dedede; font-weight: bold;"> Resumo de Obras </td>
                </tr>
                <tr>
                    <td style="padding: 0px; margin: 0px;">
                        <?
                        // cria a query com as situações de obras e a quantidade
                        $sql = "SELECT
                                    '<a href=painel.php?modulo=relatorio/relatorioObra&acao=A".(($arrUrlInterna)?implode("",$arrUrlInterna):"")."&stoid='|| id ||'>'||situacao||'</a>' AS descricao,
                                    SUM(totalobras) AS total
                                FROM(
                                    SELECT
                                        CASE oi.stoid
                                            WHEN 1 THEN '1,690' --Em Execução
                                            WHEN 2 THEN '2,691' --Paralisada
                                            WHEN 3 THEN '3,693' --Concluída
                                            WHEN 4 THEN '4' --Em Elaboração de Projetos
                                            WHEN 5 THEN '5,763' --Em Licitação
                                            WHEN 6 THEN '6,766' --Contrato Cancelado
                                            WHEN 7 THEN '7,767' --Convênio Cancelado
                                            WHEN 9 THEN '9,768' --Em Reformulação
                                            WHEN 10 THEN '10,769' --Obra Cancelada
                                            WHEN 11 THEN '11,770' --Etapa Concluída
                                            WHEN 12 THEN '12,771' --Aguardando registro de preços
                                            WHEN 99 THEN '99,689' --Em Planejamento pelo Proponente
                                        ELSE oi.stoid::varchar
                                        END AS id,
                                        so.stodesc AS situacao,
                                        count(oi.obrid) AS totalobras
                                    FROM obras.obrainfraestrutura oi
                                    LEFT JOIN obras.situacaoobra so ON so.stoid = oi.stoid
                                    LEFT JOIN entidade.entidade ent ON ent.entid = oi.entidunidade
                                    LEFT JOIN entidade.entidade ent2 ON ent2.entid = oi.entidcampus
                                    INNER JOIN entidade.endereco ed ON ed.endid = oi.endid
                                    WHERE oi.obsstatus = 'A'
                                    AND oi.orgid NOT IN (3)
                                    ".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
                                    GROUP BY id, situacao
                                UNION ALL
                                    SELECT
                                        CASE esd.esdid
                                            WHEN 690 THEN '1,690' --Em Execução
                                            WHEN 691 THEN '2,691' --Paralisada
                                            WHEN 693 THEN '3,693' --Concluída
                                            --WHEN 771 THEN 4 --Em Elaboração de Projetos
                                            WHEN 763 THEN '5,763' --Em Licitação
                                            WHEN 766 THEN '6,766' --Contrato Cancelado
                                            WHEN 767 THEN '7,767' --Convênio Cancelado
                                            WHEN 768 THEN '9,768' --Em Reformulação
                                            WHEN 769 THEN '10,769' --Obra Cancelada
                                            WHEN 770 THEN '11,770' --Etapa Concluída
                                            WHEN 771 THEN '12,771' --Aguardando registro de preços
                                            WHEN 689 THEN '99,689' --Em Planejamento pelo Proponente
                                        ELSE esd.esdid::varchar
                                        END AS id,
                                        CASE esd.esdid
                                            WHEN 690 THEN 'Em Execução'
                                            WHEN 691 THEN 'Paralisada'
                                            WHEN 693 THEN 'Concluída'
                                            --WHEN 771 THEN 4 --Em Elaboração de Projetos
                                            WHEN 763 THEN 'Em Licitação'
                                            WHEN 766 THEN 'Contrato Cancelado'
                                            WHEN 767 THEN 'Convênio Cancelado'
                                            WHEN 768 THEN 'Em Reformulação'
                                            WHEN 769 THEN 'Obra Cancelada'
                                            WHEN 770 THEN 'Etapa Concluída'
                                            WHEN 771 THEN 'Aguardando registro de preços'
                                            WHEN 689 THEN 'Em Planejamento pelo Proponente'
                                        ELSE esd.esddsc
                                        END AS situacao,
                                        count(oi.obrid) AS totalobras
                                    FROM obras2.obras oi
                                    INNER JOIN obras2.empreendimento e ON e.empid = oi.empid AND e.empstatus = 'A'
                                    INNER JOIN workflow.documento d ON d.docid = oi.docid
                                    INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
                                    LEFT JOIN entidade.entidade ent ON ent.entid = oi.entid
                                    INNER JOIN entidade.endereco ed ON ed.endid = oi.endid
                                    WHERE oi.obrstatus = 'A'
                                    AND e.orgid IN (3)
                                    AND d.esdid NOT IN (770) --Etapa Concluída
                                    AND oi.obridpai IS NULL
                                    ".(($filtro2)?" AND ".implode(" AND ", $filtro2):"")."
                                    GROUP BY id, situacao
                                ) AS FOO
                                GROUP BY id, descricao
                                ORDER BY descricao";
                        //ver($sql);
                        // exibe na tela a lista de situações de obras
                        $cabecalho = array("Situação", "Qtd de Obras");
                        $db->monta_lista_simples( $sql, $cabecalho, 1000, 30, 'S', '100%','N');
                        ?>
                    </td>
                </tr>
            </table>
		</td>
	</tr>
	<tr>
		<td>
            <table cellspacing="1" cellpadding="3" width="100%">
                <tr>
                    <td style="text-align: center; background-color: #dedede; font-weight: bold;"> Lista de obras </td>
                </tr>
                <?
                $sql = "SELECT
                            SUM(totalobras) AS total
                        FROM(
                            SELECT
                                count(oi.obrid) AS totalobras
                            FROM obras.obrainfraestrutura oi
                            LEFT JOIN obras.situacaoobra so ON so.stoid = oi.stoid
                            LEFT JOIN entidade.entidade ent ON ent.entid = oi.entidunidade
                            LEFT JOIN entidade.entidade ent2 ON ent2.entid = oi.entidcampus
                            INNER JOIN entidade.endereco ed ON ed.endid = oi.endid
                            WHERE oi.obsstatus = 'A'
                            AND oi.orgid NOT IN (3)
                            ".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
                        UNION ALL
                            SELECT
                                count(oi.obrid) AS totalobras
                            FROM obras2.obras oi
                            INNER JOIN obras2.empreendimento e ON e.empid = oi.empid AND e.empstatus = 'A'
                            INNER JOIN workflow.documento d ON d.docid = oi.docid
                            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
                            LEFT JOIN entidade.entidade ent ON ent.entid = oi.entid
                            INNER JOIN entidade.endereco ed ON ed.endid = oi.endid
                            WHERE oi.obrstatus = 'A'
                            AND e.orgid IN (3)
                            AND d.esdid NOT IN (770) --Etapa Concluída
                            AND oi.obridpai IS NULL
                            ".(($filtro2)?" AND ".implode(" AND ", $filtro2):"")."
                        ) AS FOO";
                //ver($sql,d);
                $pages = $db->pegaUm($sql);

                if( $_REQUEST['estado'] || $_SESSION['par']['estado'] ){
                    $_SESSION['par']['estado'] = 1;
                    $inner = "INNER JOIN entidade.funcaoentidade fe on fe.entid = ent.entid";
                    $where = "and funid = 6";
                }

                $sql = "SELECT
                            ent.entnome as unidade,
                            COALESCE(ent2.entnome, 'Não cadastrado') as campus,
                            oi.stoid,
                            oi.obrid,
                            oi.obrdesc as nome,
                            oi.obrdtinicio,
                            oi.obrdttermino,
                            COALESCE(tm.mundescricao,'Não cadastrado')||'/'||COALESCE(ed.estuf, 'Não cadastrado') as local,
                            case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
                            (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
                            oi.obrcomposicao,
                            CASE WHEN oi.stoid NOT IN (2,3,4,5) THEN
                            (CASE WHEN (CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) > (CURRENT_DATE - integer '30') THEN '<label style=\"color:#00AA00;\">' ||  to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
                                  WHEN ((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) <= (CURRENT_DATE - integer '30') AND (CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) >= (CURRENT_DATE - integer '45')) THEN '<label style=\"color:#BB9900;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
                             ELSE '<label style=\"color:#DD0000;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>' END)
                             WHEN oi.stoid = 2 THEN '<label style=\"color:#DD0000;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
                             WHEN oi.stoid = 3 THEN '<label style=\"color:blue;\">' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '</label>'
                            ELSE to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') END as ultimadata
                        FROM obras.obrainfraestrutura oi
                        LEFT JOIN obras.situacaoobra so ON so.stoid = oi.stoid
                        LEFT JOIN entidade.endereco ed ON ed.endid = oi.endid
                        LEFT JOIN entidade.entidade ent ON ent.entid = oi.entidunidade
                        LEFT JOIN entidade.entidade ent2 ON ent2.entid = oi.entidcampus
                        LEFT JOIN territorios.municipio tm ON tm.muncod = ed.muncod
                        ".$inner."
                        WHERE oi.obsstatus = 'A'
                        ".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
                        ".$where."
                        ORDER BY oi.orgid, ent.entnome, ent2.entnome, oi.obrdesc, so.stodesc
                        LIMIT 100 OFFSET ".$_REQUEST['pagina'];

                $sql = "SELECT *
                        FROM (
                            SELECT
                                oi.orgid,
                                ent.entnome as unidade,
                                COALESCE(ent2.entnome, 'Não cadastrado') as campus,
                                oi.stoid AS idsituacao,
                                oi.obrid,
                                oi.obrdesc as nome,
                                oi.obrdtinicio,
                                oi.obrdttermino,
                                COALESCE(tm.mundescricao,'Não cadastrado')||'/'||COALESCE(ed.estuf, 'Não cadastrado') as local,
                                case when oi.stoid is not null then so.stodesc else 'Não Informado' end as situacao,
                                (SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
                                oi.obrcomposicao,
                                CASE WHEN oi.stoid NOT IN (2,3,4,5) THEN
                                (CASE WHEN (CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) > (CURRENT_DATE - integer '30') THEN '' ||  to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || ''
                                  WHEN ((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) <= (CURRENT_DATE - integer '30') AND (CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END) >= (CURRENT_DATE - integer '45')) THEN '' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || ''
                                ELSE '' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || '' END)
                                WHEN oi.stoid = 2 THEN '' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || ''
                                WHEN oi.stoid = 3 THEN '' || to_char((CASE WHEN oi.obrdtvistoria is not null THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') || ''
                                ELSE to_char((CASE WHEN oi.obrdtvistoria IS NOT NULL THEN oi.obrdtvistoria ELSE oi.obsdtinclusao END), 'DD/MM/YYYY') END as ultimadata
                            FROM obras.obrainfraestrutura oi
                            LEFT JOIN obras.situacaoobra so ON so.stoid = oi.stoid
                            LEFT JOIN entidade.endereco ed ON ed.endid = oi.endid
                            LEFT JOIN entidade.entidade ent ON ent.entid = oi.entidunidade
                            LEFT JOIN entidade.entidade ent2 ON ent2.entid = oi.entidcampus
                            LEFT JOIN territorios.municipio tm ON tm.muncod = ed.muncod
                            ".$inner."
                            WHERE oi.obsstatus = 'A'
                            AND oi.orgid NOT IN (3)
                            ".(($filtro)?" AND ".implode(" AND ", $filtro):"")."
                            ".$where."
                        UNION ALL
                            SELECT
                                e.orgid,
                                ent.entnome as unidade,
                                'Não cadastrado' as campus,
                                d.esdid AS idsituacao,
                                oi.obrid,
                                oi.obrnome as nome,
                                oi.obrdtinicio,
                                oi.obrdtfim AS obrdttermino,
                                COALESCE(tm.mundescricao,'Não cadastrado')||'/'||COALESCE(ed.estuf, 'Não cadastrado') as local,
                                esd.esddsc as situacao,
                                --(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
                                replace((((((100 - coalesce(oi.obrperccontratoanterior,0)) * coalesce(oi.obrpercentultvistoria,0)) / 100) + coalesce(oi.obrperccontratoanterior,0))::numeric(20,2))::text || ' %', '.', ',') AS percentual,
                                e.empcomposicao AS obrcomposicao,
                                CASE WHEN esd.esdid NOT IN (691,693,4,763) THEN
                                (CASE WHEN (CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END) > (CURRENT_DATE - integer '30') THEN '' ||  to_char((CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END), 'DD/MM/YYYY') || ''
                                  WHEN ((CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END) <= (CURRENT_DATE - integer '30') AND (CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END) >= (CURRENT_DATE - integer '45')) THEN '' || to_char((CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END), 'DD/MM/YYYY') || ''
                                ELSE '' || to_char((CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END), 'DD/MM/YYYY') || '' END)
                                WHEN esd.esdid = 691 THEN '' || to_char((CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END), 'DD/MM/YYYY') || ''
                                WHEN esd.esdid = 693 THEN '' || to_char((CASE WHEN oi.obrdtultvistoria is not null THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END), 'DD/MM/YYYY') || ''
                                ELSE to_char((CASE WHEN oi.obrdtultvistoria IS NOT NULL THEN oi.obrdtultvistoria ELSE oi.obrdtinclusao END), 'DD/MM/YYYY') END as ultimadata
                            FROM obras2.obras oi
                            INNER JOIN obras2.empreendimento e ON e.empid = oi.empid AND e.empstatus = 'A'
                            INNER JOIN workflow.documento d ON d.docid = oi.docid
                            INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
                            LEFT JOIN entidade.entidade ent ON ent.entid = oi.entid
                            INNER JOIN entidade.endereco ed ON ed.endid = oi.endid
                            LEFT JOIN territorios.municipio tm ON tm.muncod = ed.muncod
                            WHERE oi.obrstatus = 'A'
                            AND e.orgid IN (3)
                            AND d.esdid NOT IN (770) --Etapa Concluída
                            AND oi.obridpai IS NULL
                            ".(($filtro2)?" AND ".implode(" AND ", $filtro2):"")."
                            ".$where."
                        ) AS FOO
                        ORDER BY orgid, unidade, campus, nome, situacao
                        LIMIT 100 OFFSET ".$_REQUEST['pagina'];
                //ver($sql,d);
                $obras = $db->carregar($sql);
                if($pages > 100) {
                    echo "<tr><td>Paginação: ";
                    for($i=0;$i<ceil($pages/100);$i++) {

                        if($_REQUEST['pagina']==$i) $label = "<b>".($i+1)."</b>";
                        else $label = ($i+1);

                        echo "<a href=painel.php?modulo=relatorio/relatorioObra&acao=A".(($arrUrlInterna)?implode("",$arrUrlInterna):"")."&pagina=".$i.">".($label)."</a> ";
                    }
                    echo "</td></tr>";
                }

                if($obras[0]) {
                    foreach($obras as $obra) {
                        switch ( $obra["stoid"] ){
                            case "1":
                                $obra['situacao'] = '<label style="color:#00AA00">' . $obra['situacao'] . '</label>';
                            break;
                            case "690":
                                $obra['situacao'] = '<label style="color:#00AA00">' . $obra['situacao'] . '</label>';
                                break;
                            case "2":
                                $obra['situacao'] = '<label style="color:#DD0000">' . $obra['situacao'] . '</label>';
                            break;
                            case "691":
                                $obra['situacao'] = '<label style="color:#DD0000">' . $obra['situacao'] . '</label>';
                                break;
                            case "3":
                                $obra['situacao'] = '<label style="color:blue">' . $obra['situacao'] . '</label>';
                            break;
                            case "693":
                                $obra['situacao'] = '<label style="color:blue">' . $obra['situacao'] . '</label>';
                                break;
                            case "6":
                                $obra['situacao'] = '<label style="color:#DD0000">' . $obra['situacao'] . '</label>';
                                break;
                            case "766":
                                $obra['situacao'] = '<label style="color:#DD0000">' . $obra['situacao'] . '</label>';
                                break;
                        }

                        echo "<tr><td>

                        <table class=listagem width=100%>
                            <tr>
                                <td width=15% class=SubTituloDireita>Unidade:</td>
                                <td width=35%>".strtoupper($obra['unidade'])."</td>
                                <td width=15% class=SubTituloDireita>Campus:</td>
                                <td width=35%>".strtoupper($obra['campus'])."</td>
                            </tr>
                            <tr>
                                <td width=15% class=SubTituloDireita>Nome da obra:</td>
                                <td width=35%>".$obra['nome']."</td>
                                <td width=15% class=SubTituloDireita>Municipio/UF:</td>
                                <td width=35%>".$obra['local']."</td>
                            </tr>

                            <tr>
                                <td width=15% class=SubTituloDireita>Início programado:</td>
                                <td width=35%>".(($obra['obrdtinicio'])?formata_data($obra['obrdtinicio']):"Não cadastrado")."</td>
                                <td width=15% class=SubTituloDireita>Término programado:</td>
                                <td width=35%>".(($obra['obrdttermino'])?formata_data($obra['obrdttermino']):"Não cadastrado")."</td>
                            </tr>

                            <tr>
                                <td width=15% class=SubTituloDireita>Situação da Obra:</td>
                                <td width=35%>".$obra['situacao']."</td>
                                <td width=15% class=SubTituloDireita>% Executado:</td>
                                <td width=35%>".$obra['percentual']."</td>
                            </tr>


                            <tr>
                                <td width=15% class=SubTituloDireita>Última atualização:</td>
                                <td colspan=3>".$obra['ultimadata']."</td>
                            </tr>

                            <tr>
                                <td width=15% class=SubTituloDireita>Descrição:</td>
                                <td width=85% colspan=3>".(($obra['obrcomposicao'])?nl2br($obra['obrcomposicao']):"Não cadastrado")."</td>
                            </tr>

                            <tr>
                                <td class=SubTituloCentro colspan=4>FOTOS:</td>
                            </tr>
                        ";

                        $sql = "SELECT *
                                FROM (
                                    SELECT
                                        arqnome,
                                        arq.arqid,
                                        arq.arqextensao,
                                        arq.arqtipo,
                                        arq.arqdescricao,
                                        to_char(oar.aqodtinclusao,'dd/mm/yyyy') AS aqodtinclusao
                                    FROM public.arquivo arq
                                    INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
                                    INNER JOIN obras.obrainfraestrutura obr ON obr.obrid = oar.obrid
                                    WHERE obr.obrid='". $obra['obrid'] ."'
                                    AND aqostatus = 'A'
                                    AND obr.orgid NOT IN (3)
                                    AND (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
                                UNION ALL
                                    SELECT
                                        arqnome,
                                        arq.arqid,
                                        arq.arqextensao,
                                        arq.arqtipo,
                                        arq.arqdescricao,
                                        to_char(oar.aqodtinclusao,'dd/mm/yyyy') AS aqodtinclusao
                                    FROM public.arquivo arq
                                    INNER JOIN obras.arquivosobra oar ON arq.arqid = oar.arqid
                                    INNER JOIN obras2.obras obr ON obr.obrid = oar.obrid
                                    WHERE obr.obrid='". $obra['obrid'] ."'
                                    AND aqostatus = 'A'
                                    AND (arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png')
                                ) AS FOO
                                ORDER BY arqid DESC
                                LIMIT 4";
                        $fotos = $db->carregar($sql);
                        echo "<tr><td valign=\"top\" align=\"center\" colspan=4><table><tr>";
                        if($fotos[0]){
                            for( $k = 0; $k < count($fotos); $k++ ){
                                $_SESSION['imgparametos'][$fotos[$k]["arqid"]] = array( "filtro" => "cnt.obrid=".$obra['obrid']." AND aqostatus = 'A'", "tabela" => "obras.arquivosobra");
                                echo "<td valign=\"top\" align=\"center\">";
                                echo "<img id='".$fotos[$k]["arqid"]."' onclick='window.open(\"../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&obrid={$obra['obrid']}\",\"imagem\",\"width=850,height=600,resizable=yes\");' src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras&newwidth=120&newheight=90&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='width:80px; height:80px;'/><br />".$fotos[$k]["aqodtinclusao"]."<br />".$fotos[$k]["arqdescricao"];
                                echo "</td>";
                            }
                        } else {
                            echo "<td>Não existe(m) foto(s) cadastrada(s).</td>";
                        }
                        echo "</tr></table></td></tr>";
                        echo "</table>
                        </td></tr>";
                    }
                } else {
                    echo "<tr><td>Não existem obras cadastradas</td></tr>";
                }
                ?>
			</table>
		</td>
	</tr>
</table>
</body>
</html>