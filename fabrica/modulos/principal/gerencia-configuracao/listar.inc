<?php
header( 'content-type: text/html; charset=iso-8859-1;' );
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

// Monta as Abas da Página
$menu[0] = array( "descricao" => "Auditoria GC", "link" => "fabrica.php?modulo=principal/gerencia-configuracao/listar&acao=A" );
echo montarAbasArray( $menu, "fabrica.php?modulo=principal/gerencia-configuracao/listar&acao=A" );

/*
 * SS-982 - REQ004 - GC - Consultar Solicitação
 * */
$pfls = arrayPerfil();
$situacaoOrdemDeServicoPorPerfil = array( WF_ESTADO_OS_VERIFICACAO
										, WF_ESTADO_OS_APROVACAO
										, WF_ESTADO_OS_ATESTO_TECNICO
										, WF_ESTADO_OS_AGUARDANDO_PAGAMENTO
										, WF_ESTADO_OS_FINALIZADA );

if( in_array(PERFIL_ESPECIALISTA_SQUADRA, $pfls) || in_array(PERFIL_PREPOSTO,  $pfls)  ) 
{
	array_push( $situacaoOrdemDeServicoPorPerfil, WF_ESTADO_OS_EXECUCAO );
	$situacaoOrdemDeServicoPorPerfil = implode(',', $situacaoOrdemDeServicoPorPerfil);
	
}else{
	
	$situacaoOrdemDeServicoPorPerfil = implode(',', $situacaoOrdemDeServicoPorPerfil);
}

// Monta o Titulo da página abaixo da Aba
monta_titulo( "Consultar solicitação de serviço", "<b>Filtros</b>" );

//Montando Lista de Sistemas
$sistemaRepositorio = new SistemaRepositorio();
$sistemasAtivos     = $sistemaRepositorio->recuperaTodosAtivos();

//Montando Lista de requisitantes
//$requisitanteRepositorio = new RequisitanteRepositorio();
//$requisitantes = $requisitanteRepositorio->recuperaTodos();
//Montado Lista de Fiscais
$fiscalRepositorio = new FiscalRepositorio();
$fiscais           = $fiscalRepositorio->recupereTodos();

//Montando Lista de Status de SS
$situacaoSolicitacaoRepositorio = new SituacaoSolicitacaoRepositorio();
$situacoesSolicitacao           = $situacaoSolicitacaoRepositorio->recupereTodos();

//Montando Lista de Solicitacoes
$solicitacaoRepositorio = new SolicitacaoRepositorio();

$cpf = $_SESSION['usucpf'];

$orderBy = $_POST['campo'] == null ? 'scsid' : $_POST['campo'] ;
$ordem = $_POST['ordem'] == null ? Ordenador::ORDEM_PADRAO : $_POST['ordem'];
$limit = $_POST['limit'] == null ? Paginador::LIMIT_PADRAO : $_POST['limit'];
$offset = $_POST['offset'] == null ? Paginador::OFFSET_PADRAO : $_POST['offset'];

if ($_GET){
	if($_GET['redirecionamento'] && isset($_SESSION['form-listaAuditoriaGc'])){
		extract($_SESSION['form-listaAuditoriaGc']);
		$_POST = $_SESSION['form-listaAuditoriaGc'];
	} else {
		unset($_SESSION['form-listaAuditoriaGc']);
	}
}

$sql = "SELECT DISTINCT '<a href=\"?modulo=principal/gerencia-configuracao/listarArtefatos&acao=A&solicitacao=' || scsid || '\"><img style=\"border: none;\" src=\"/imagens/consultar.gif\" alt=\"Visualizar\" title=\"Visualizar\"/></a>' as imagem, 
					scsid, siddescricao, nomefiscal, esddsc, '<span style= \"display:none;\">' || to_char(dataabertura,'YYYY-MM-DD')  ||'</span>' || to_char(dataabertura,'DD/MM/YYYY') as dataabertura
                    , '<span style= \"display:none;\">' || to_char(data_finalizacao,'YYYY-MM-DD')  ||'</span>' || to_char(data_finalizacao,'DD/MM/YYYY') as data_finalizacao,  
					quantidadecontratados, 
					(CASE quantidadeauditados WHEN null THEN '0' ELSE quantidadeauditados END) as quantidadeauditados, (quantidadeauditados * 100 / quantidadecontratados) || '%' as perc_auditada
		FROM (SELECT DISTINCT ss.scsid, ss.odsidorigem, ss.prgid, ss.prgcod, ss.prgano, ss.usucpfrequisitante
                                    , ss.docid, ss.scsnecessidade, ss.scsjustificativa, ss.scsprevatendimento
                                    , ss.scsstatus, ss.scsatesto, ss.scsdtatesto, ss.usucpforigem, ss.sidid
                                    , u.usunome, sd.siddescricao
                                    , ss.dataabertura, est.esdid, est.esddsc,ans.ansid, data_finalizacao.data_finalizacao
                                    , produtoscontratados.quantidadecontratados, produtosauditados.quantidadeauditados                                    
                                    , auditor.usucpf as cpfauditor, auditor.usunome as nomefiscal
                            FROM fabrica.solicitacaoservico ss 	  
                            INNER JOIN fabrica.analisesolicitacao ans 
                                ON ss.scsid = ans.scsid 
                            INNER JOIN workflow.documento doc 
                                ON doc.docid = ss.docid 
                            INNER JOIN workflow.estadodocumento est 
                                ON est.esdid = doc.esdid
                            INNER JOIN demandas.sistemadetalhe sd
                                ON ss.sidid = sd.sidid
                            INNER JOIN seguranca.usuario u
                                ON ss.usucpfrequisitante = u.usucpf
                            INNER JOIN fabrica.ordemservico os
                                ON ss.scsid = os.scsid
                            INNER JOIN workflow.documento docos
                                ON os.docid = docos.docid
                                AND docos.esdid IN ( {$situacaoOrdemDeServicoPorPerfil} )
                            INNER JOIN workflow.estadodocumento esdos
                                ON docos.esdid = esdos.esdid

                            LEFT JOIN fabrica.servicofaseproduto sfp 
                                ON sfp.ansid = ans.ansid 
                            LEFT  JOIN fabrica.fasedisciplinaproduto fdp 
                                ON sfp.fdpid = fdp.fdpid 
                            LEFT  JOIN fabrica.produto p 
                                ON fdp.prdid = p.prdid 
                            LEFT  JOIN fabrica.disciplina dis 
                                ON p.dspid = dis.dspid	  
                            LEFT JOIN fabrica.auditoria aud
                                ON ans.ansid = aud.ansid
                            LEFT JOIN seguranca.usuario auditor
                                ON aud.usucpf = auditor.usucpf

                            LEFT JOIN ( SELECT produtos.scsid , count(*) as quantidadecontratados
                                FROM (SELECT DISTINCT fdp.prdid, ans.scsid, pro.prddsc, pro.prdstatus, dis.dspid, dis.dspdsc
                                    FROM fabrica.fasedisciplinaproduto fdp
                                    JOIN fabrica.fasedisciplina fd
                                        ON fdp.fsdid = fd.fsdid
                                    JOIN fabrica.produto pro
                                        ON fdp.prdid = pro.prdid
                                    JOIN fabrica.disciplina dis
                                        ON dis.dspid = fd.dspid
                                    JOIN fabrica.servicofaseproduto sfp
                                        ON sfp.fdpid = fdp.fdpid
                                    JOIN fabrica.analisesolicitacao ans
                                        ON ans.ansid = sfp.ansid
                                    WHERE pro.prdid <> 44 and sfp.tpeid = 1) as produtos 
                                    group by scsid ) as produtoscontratados
                                ON ss.scsid = produtoscontratados.scsid 


                            LEFT JOIN (SELECT count(*) as quantidadeauditados, tmp.scsid 
                                    FROM (SELECT DISTINCT fdp.prdid, sss.scsid, pro.prddsc, pro.prdstatus,dis.dspid
                                                , dis.dspdsc
                                        FROM fabrica.fasedisciplinaproduto fdp
                                        JOIN fabrica.fasedisciplina fd
                                            ON fdp.fsdid = fd.fsdid
                                        JOIN fabrica.produto pro
                                            ON fdp.prdid = pro.prdid
                                        JOIN fabrica.disciplina dis
                                            ON dis.dspid = fd.dspid
                                        JOIN fabrica.servicofaseproduto sfp
                                            ON sfp.fdpid = fdp.fdpid
                                        JOIN fabrica.detalhesauditoria deta 
                                            ON sfp.sfpid = deta.sfpid
                                            AND deta.dtaresultado <> 0
                                        JOIN fabrica.analisesolicitacao ans
                                            ON ans.ansid = sfp.ansid
                                        JOIN fabrica.solicitacaoservico sss
                                            ON ans.scsid = sss.scsid
                                        JOIN fabrica.detalhesauditoria dtaud
                                            ON dtaud.sfpid = sfp.sfpid
                                        WHERE pro.prdid <> 44 and ans.scsid = sss.scsid and sfp.tpeid = 1) as tmp 
                                    GROUP BY scsid) as produtosauditados 
                                ON ss.scsid = produtosauditados.scsid 


                            LEFT JOIN ( SELECT sss.scsid, MAX(htddata) AS data_finalizacao 
                                        FROM fabrica.solicitacaoservico sss 
                                        JOIN workflow.documento d 
                                            ON d.docid = sss.docid 
                                            AND d.esdid = ". WF_ESTADO_FINALIZADA ."
                                        JOIN workflow.historicodocumento hsd 
                                            ON d.docid = hsd.docid 
                                        JOIN workflow.acaoestadodoc aed 
                                            ON hsd.aedid = aed.aedid 
                                        WHERE aed.esdidorigem IN ( ". WF_ESTADO_AGUARDANDO_PAGAMENTO .", ". WF_ESTADO_EXECUCAO ." )
                                            AND aed.esdiddestino = ". WF_ESTADO_FINALIZADA ." 
                                        GROUP BY sss.scsid) as data_finalizacao
                                ON ss.scsid = data_finalizacao.scsid				
                        WHERE est.esdid IN ( ". WF_ESTADO_EXECUCAO .", ". WF_ESTADO_FINALIZADA ." ) 
                        ORDER BY ss.scsid) as solicitacao";

$where  = array();

if ( $_POST['scsid'] != null )
{
	$idSolicitacao  = $_POST['scsid'];
	$where[]        = "scsid = $idSolicitacao";
}

if ( $_POST['sistemas'] != null ){
	$idSistema = $_POST['sistemas'];
	$where[] = "sidid = $idSistema";
}

if ( $_POST['fiscaiss'] != null ){
	$cpfFiscal = $_POST['fiscaiss'];
	$where[] = "cpfauditor = '$cpfFiscal'";
}

if ( $_POST['situacoes'] != null )
{
	$situacao = $_POST['situacoes'];
	$where[] = "esdid = $situacao";
}

if ( $_POST['data_abertura_inicio']!=null )
{
	$dataAberturaInicio = DateTimeUtil::retiraMascara($_POST['data_abertura_inicio']);
	$where[] = "dataabertura::date >= '$dataAberturaInicio'::date";
}

if ( $_POST['data_abertura_fim']!=null )
{
	$dataAberturaInicio = DateTimeUtil::retiraMascara($_POST['data_abertura_fim']);
	$where[] = "dataabertura::date <= '$dataAberturaInicio'::date";
}

if ( $_POST['data_finalizacao_inicio'] != null )
{
	$dataFinalizacaoInicio = DateTimeUtil::retiraMascara($_POST['data_finalizacao_inicio']);
	$where[] = "(
					SELECT htddata
					from fabrica.solicitacaoservico sss 
					join workflow.documento d 
						on d.docid = solicitacao.docid 
					join workflow.historicodocumento hsd 
						on d.docid = hsd.docid 
					join workflow.acaoestadodoc aed 
						on hsd.aedid = aed.aedid 
					where
						sss.scsid = solicitacao.scsid 
                        AND aed.esdidorigem  = ". WF_ESTADO_AGUARDANDO_PAGAMENTO ." 
                        AND aed.esdiddestino = ". WF_ESTADO_FINALIZADA ."
					ORDER BY htddata DESC LIMIT 1
				)::date >= '$dataFinalizacaoInicio'::date";
}

if ($_POST['data_finalizacao_fim']!=null){
	$dataFinalizacaoFim = DateTimeUtil::retiraMascara($_POST['data_finalizacao_fim']);
	$where[] = "(
					SELECT htddata
					FROM fabrica.solicitacaoservico sss 
					JOIN workflow.documento d 
						on d.docid = solicitacao.docid 
					JOIN workflow.historicodocumento hsd 
						on d.docid = hsd.docid 
					JOIN workflow.acaoestadodoc aed 
						on hsd.aedid = aed.aedid 
					WHERE
						sss.scsid = solicitacao.scsid 
                        AND aed.esdidorigem  = ". WF_ESTADO_AGUARDANDO_PAGAMENTO ." 
                        AND aed.esdiddestino = ". WF_ESTADO_FINALIZADA ."
					ORDER BY htddata DESC LIMIT 1
				) <= '$dataFinalizacaoFim'";
}

if ( $_POST['minhasAuditorias'] == "S" )
{
	$where[] = "cpfauditor = '$cpf'";
	$checked = "checked=\"checked\"";
} else {
	$checked = '';	
}

if( !$_POST['todos'] )
{
    $where[] =  "(cpfauditor = '{$cpf}' OR cpfauditor is null)";
}

if( !empty($where) ){
	$sql .= " WHERE " . implode(" AND ", $where);
}

$sql .= " ORDER BY 2 ASC ";

if ($_POST) {
	extract($_POST);
	$_SESSION['form-listaAuditoriaGc'] = $_POST;
	
	if (!empty($data_abertura_inicio)) {
		$data_abertura_inicio = substr($data_abertura_inicio, 6, 4) . "-" . substr($data_abertura_inicio, 3, 2) . "-" . substr($data_abertura_inicio, 0, 2);
	}
	if (!empty($data_abertura_fim)) {
		$data_abertura_fim = substr($data_abertura_fim, 6, 4) . "-" . substr($data_abertura_fim, 3, 2) . "-" . substr($data_abertura_fim, 0, 2);
	}
	if (!empty($data_finalizacao_inicio)) {
		$data_finalizacao_inicio = substr($data_finalizacao_inicio, 6, 4) . "-" . substr($data_finalizacao_inicio, 3, 2) . "-" . substr($data_finalizacao_inicio, 0, 2);
	}
	if (!empty($data_finalizacao_fim)) {
		$data_finalizacao_fim = substr($data_finalizacao_fim, 6, 4) . "-" . substr($data_finalizacao_fim, 3, 2) . "-" . substr($data_finalizacao_fim, 0, 2);
	}
}


?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>

<form id="form-listaAuditoriaGc" method="post" action="?modulo=principal/gerencia-configuracao/listar&acao=A">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" width="25%">Número de solicitação:</td>
            <td>
                <?php echo campo_texto( 'scsid', 'N', 'S', "", 11, '', '######', '', '', '', 0, 'id="scsid"', '', "", '', '' );?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Sistema:</td>
            <td>
                <select id="gc-sistemas" class="campoEstilo" name="sistemas" style="width: auto;">
                    <option id="gc-fiscais-sistemas" value="">Selecione</option>
					<?php foreach ( $sistemasAtivos as $sa ) { ?>
                        <option value="<?php echo $sa->getId(); ?>" <?php echo $sa->getId() == $sistemas ? "selected=selected" : "" ;?>><?php echo $sa->getDescricao(); ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Fiscal:</td>
            <td>
                <select id="gc-fiscais" class="campoEstilo" name="fiscaiss" style="width: auto;">
                    <option id="gc-fiscais-selecione" value="">Selecione</option>
                    <?php foreach ( $fiscais as $fiscal ) { ?>
                        <option value="<?php echo $fiscal->getId(); ?>" <?php echo $fiscal->getId() == $fiscaiss ? "selected=selected" : "" ;?>><?php echo $fiscal->getNome(); ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Situação SS:</td>
            <td>
                <select id="gc-situacoes" class="campoEstilo" name="situacoes" style="width: auto;">
                    <option id="gc-situacoes-selecione" value="">Selecione</option>
                    <?php 
                    foreach ( $situacoesSolicitacao as $situacao ) { 
                    	if( $situacao->getId() != WF_ESTADO_AGUARDANDO_PAGAMENTO ) {
                    	?>
                        <option value="<?php echo $situacao->getId(); ?>" <?php echo $situacao->getId() == $situacoes ? "selected=selected" : "" ;?>><?php echo $situacao->getDescricao(); ?></option>
						<?php 
                    	}
                    } 
                    ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Data da abertura:</td>
            <td>
            	
                <?php echo campo_data2( 'data_abertura_inicio', 'N', 'S', 'Data de Abertura', 'S', '', '', '' ); ?>
                <span> à </span>
                <?php echo campo_data2( 'data_abertura_fim', 'N', 'S', 'Data de Abertura', 'S', '', '', '' ); ?>
                <label id="error_primeiro_periodo" class="error" style="display: none;">Período inválido</label>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Data da finalização:</td>
            <td>
                <?php echo campo_data2( 'data_finalizacao_inicio', 'N', 'S', 'Data de Finalização', 'S', '', '', '' ); ?>
                <span> à </span>
                <?php echo campo_data2( 'data_finalizacao_fim', 'N', 'S', 'Data de Finalização', 'S', '', '', '' ); ?>
                <label id="error_segundo_periodo" class="error" style="display: none;">Período inválido</label>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%"></td>
            <td>
                <input id="gc-minhasAuditorias" type="checkbox" name="minhasAuditorias" value="S" <?php echo $checked;?>/>Minhas auditorias
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%"></td>
            <td>
                <input id="submit-listaAuditoriaGc" type="submit" value="Consultar" />
                <input id="reset-limparAuditoriaGc" type="reset" value="Limpar"/>
                <input id="submit-listaTodasSSGc" type="submit" name="todos" value="Todos"/>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloCentro" align="center">Lista de Solicitações</td>
        </tr>
    </table>
</form>
<?php
	//die( $sql );
	$cabecalho     = array('Ação','SS','Sistema','Fiscal','Situação SS','Abertura','Finalização','Artefatos','Auditados','%Auditada');
	$tamanho       = array("5%","5%","20%","30%","15%","5%","10%","10%", '5%', '5%');
	$alinhamento   = array("center","center","left","left","left","center","center","center", 'center', 'center');
	$db->monta_lista($sql,$cabecalho,20,5,'N','center','',"",$tamanho,$alinhamento);
?>
<script type="text/javascript" src="./js/gerencia-configuracao.js"></script>
