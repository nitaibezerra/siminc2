<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$obras = array();

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 4;

/****************************************
*				PREFEITO				*
****************************************/

// Pendências do município > 60 dias
$dados = getDados();

enviarNotificacoes($dados);

//registraAtividade($obras);

echo "FIM";

function getDados()
{
	$db = new cls_banco();

    $sql = "select distinct sisabrev, e.sisid, errdescricao,
                case
                    when errtipo = 'DB' then 'Banco de dados'
                    when errtipo = 'PR' then 'Programação'
                    when errtipo = 'QB' then 'Queda no banco'
                    when errtipo = 'WS' then 'WebService'
                    when errtipo = 'EN' then 'Encoding no banco'
                    when errtipo = 'PD' then 'Erro na Conexão'
                    when errtipo = 'DC' then 'Diretório Cheio'
                    when errtipo = 'AI' then 'Arquivo inexistente'
                    when errtipo = 'DV' then 'Diversos'
                    else errtipo
                end as descricao,
                errarquivo, errlinha, count(*)
        from seguranca.erro e
                left join seguranca.sistema s on s.sisid = e.sisid
        where to_char(errdata, 'YYYY-MM-DD') = to_char(NOW(), 'YYYY-MM-DD')
        group by errdescricao, errtipo, sisabrev, e.sisid, errarquivo, errlinha, errdata
        order by sisabrev, errarquivo, errlinha
        ";
    
    $dados = $db->carregar($sql);

    $aDados = array();
    if($dados && is_array($dados)){
        foreach($dados as $count => $dado){
            $chave = $dado['sisid'] . $dado['errlinha'] . $dado['errarquivo'];
            $qtd[$chave]++;
            $aDados[$dado['sisid']][$chave] = $dado;
            $aDados[$dado['sisid']][$chave]['errlinha'] = $dado['errlinha'] . '&nbsp;';
            $aDados[$dado['sisid']][$chave]['count'] = $qtd[$chave] . '&nbsp;';
            unset($aDados[$dado['sisid']][$chave]['sisid']);
        }
    }
    return $aDados;
}

function enviarNotificacoes($dados)
{
    if($dados && is_array($dados)){
        $aDadosGeral = array();
        foreach($dados as $sisid => $aDados){

            $aDadosGeral += $aDados;

            if ($sisid) {
                $lista = recuperarLista($aDados);
                enviarEmailIndividual($lista, $sisid);
            }
        }

        $lista = recuperarLista($aDadosGeral);
        enviarEmailGeral($lista);
    }
}

function recuperarLista($aDados){

    $db = new cls_banco();

    $aDados = array_values($aDados);
    ob_start();
    echo '<h3 style="margin-top: 20px; color: red; text-align: center;">Erros ÚNICOS de hoje (' . date('d/m/Y') . ')</h3>';
    $cabecalho = array('Módulo', 'Erro', 'Tipo', 'Arquivo', 'Linha', 'Qtd.');
    $db->monta_lista($aDados, $cabecalho, 50000000, 4, 'N','Center');
    $lista = ob_get_contents();
    ob_end_clean();

    return $lista;
}


function enviarEmailGeral($lista)
{
    $remetente     = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");
    $destinatarios = recuperarDestinatariosPadrao();

    $assunto = "Relatório diário de erros únicos";

    $mensagem = "<pre>Prezados,

Segue abaixo a listagem de erros únicos de todos os módulos do SIMEC.

Cada analista receberá um email com a listagem dos erros referentes aos módulos que lhes competem para a devida correção.

{$lista}

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";
    return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
}

function enviarEmailIndividual($lista, $sisid)
{
    $db = new cls_banco();

    $remetente = array("nome"=>SIGLA_SISTEMA, "email"=>"noreply@mec.gov.br");

    $sql = "select sisabrev, sisdsc, usucpfanalista, usunome, u.usuemail
            from seguranca.sistema s
                left join seguranca.usuario u on u.usucpf = s.usucpfanalista
            where sisid = {$sisid}";
    $dados = $db->pegaLinha($sql);

    $destinatarios = array($_SESSION['email_sistema']);
    if($dados['usuemail']){
        $destinatarios[] = $dados['usuemail'];
    }

    $assunto = "Relatório diário de erros únicos do sistema {$dados['sisabrev']} ";

    $mensagem = "<pre>Prezado analista {$dados['usunome']},

Segue abaixo a listagem de erros únicos do módulo {$dados['sisabrev']}, de sua responsabilidade.

Favor corrigir os erros.

{$lista}

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";

    return enviar_email($remetente, $destinatarios, $assunto, $mensagem);
}

function recuperarDestinatariosPadrao()
{
    return array($_SESSION['email_sistema']);
}

?>