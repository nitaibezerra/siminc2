<?php
require_once '_funcoes_termoCooperacao.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
monta_titulo("Relatório Cumprimento Execução", '');

if($_SESSION['usucpf'] != '' && $_SESSION['sisid'] != ''){
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
}

function equipeTecnicaProponente() {
    global $db;

    $strSQL = "
        select * from monitora.termocooperacao where ungcodproponente in (
            select ungcod from elabrev.usuarioresponsabilidade
            where usucpf = '{$_SESSION['usucpf']}' and pflcod = ".UO_EQUIPE_TECNICA."
        );
    ";

    $linha = $db->pegaLinha($strSQL);
    return ($linha) ? true : false;
}

if (possui_perfil(array(UO_EQUIPE_TECNICA)) && !equipeTecnicaProponente()) {
    echo "<script type='text/javascript'>
        alert('Voce não possuí permissão para acessar este relatório');
        location.href='/elabrev/elabrev.php?modulo=inicio&acao=C';
    </script>";
}

?>
<form name="formulario" id="formulario" action="" method="post">
    <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
    <table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" > 
        <tr align="center">
            <td lign="center">
                <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="exibeRelatorioGeral('visual');"/>
                <input type="button" name="Gerar Relatório" value="Visualizar XLS"  onclick="exibeRelatorioGeral('xls');"/>
            </td>
        </tr>
    </table>
</form>
<?php
global $db;
$sql = "SELECT 
	numero_termo,
	unidade_proponente,
	unidade_concedente,
	identificacao_objeto,
	to_char(data_execucao, 'DD/MM/YYYY'),
	prazo_cumprimento,	
	to_char(data_execucao::date + (prazo_cumprimento || ' month')::interval , 'DD/MM/YYYY') as data_com_prazo,	
	to_char( (data_execucao::date+(prazo_cumprimento || ' month')::interval)::date+60, 'DD/MM/YYYY') as data_com_prazo_mais_60_dias
	
FROM (

	SELECT 
		tcp.tcpid as numero_termo,
		unp.ungdsc as unidade_proponente,
		unc.ungdsc as unidade_concedente,
		replace(tcpdscobjetoidentificacao, ' ? ', '') as identificacao_objeto,

		(select 
			max(hst.htddata) 
		from workflow.historicodocumento hst 
		where aedid in (select aed.aedid from workflow.acaoestadodoc aed  where aed.aedstatus = 'A' and esdiddestino = 639)
		and hst.docid = doc.docid) as data_execucao,

		(select distinct 
			max(crdmesexecucao)
		from monitora.previsaoorcamentaria pro 
		where pro.tcpid = tcp.tcpid 
		and pro.prostatus = 'A')  as prazo_cumprimento
		
	FROM monitora.termocooperacao tcp
	JOIN unidadegestora unp ON tcp.ungcodproponente = unp.ungcod
	JOIN unidadegestora unc ON tcp.ungcodconcedente = unc.ungcod
	JOIN workflow.documento doc ON doc.docid = tcp.docid
	WHERE tcp.tcpstatus = 'A'
	AND doc.esdid = 639	

) AS foo
ORDER BY numero_termo";

if ($_REQUEST['tiporelatorio']) {
    if ($_REQUEST['tiporelatorio'] == 'visual') {
        $cabecalho = array("Termo", "Unidade Proponente", "Unidade Concedente", "Objeto",
            "Data de Execução", "Prazo para o cumprimento", "Data com Prazo", "Data com Prazo + 60 dias");
        $db->monta_lista($sql, $cabecalho, 100000, 5, 'N', '100%', '');
    }
    switch ($_REQUEST['tiporelatorio']) {
        case 'xls':
            //$sql = monta_sql();
            ob_clean();
            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/xls; name=rel_unidadegestora_" . date("Ymdhis") . ".xls");
            header("Content-Disposition: attachment; filename=rel_unidadegestora_" . date("Ymdhis") . ".xls");
            header("Content-Description: MID Gera excel");

            $cabecalho = array("Termo", "Unidade Proponente", "Unidade Concedente", "Objeto",
                "Data de Execução", "Prazo para o cumprimento", "Data com Prazo", "Data com Prazo + 60 dias");

            $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%', '');
            exit;
    }
}
?>
<script type="text/javascript">
    document.getElementById('aguarde').style.display = 'none';

    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        window.location = 'elabrev.php?modulo=relatorio/termoCooperacao/relExecucao&acao=A&tiporelatorio=' + tipo;
        formulario.target = 'relatorio';
    }
</script>
