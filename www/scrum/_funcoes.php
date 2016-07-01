<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Processa as requisições ($_POST) realizadas para a página e, após validar o tipo
 * da requisição, a encaminha para processamento.
 * Quando é feita uma requisição do tipo carregar, a função retorna um array com os dados
 * solicitados, nos demais casos ela é finalizada e ocorre um redirecionamento para a página
 * que originou a requisição.
 * @param string $posfixo
 *      Classificação da requisição: programa|subprograma|estoria|entregavel
 * @return null|array
 */
function processaRequisicao($posfixo)
{
    /**
     * Requerindo funções adicionais com base na requisição recebida.
     */
    require_once("funcoes{$posfixo}.php");
    
    $retorno = null;
    if (isset($_POST['action'])) {
        // -- Limpando parametros de buscas anteriores
        limpaURI();

        // -- Montando o nome da função que será executada.
        $nomeFuncao = "{$_POST['action']}{$posfixo}";
        switch ($_POST['action']) {
            case 'carregar':
                // -- no-break
            case 'salvar':
                // -- Função não implementada
                if (!is_callable($nomeFuncao)) {
                    $msgAlerta = 'Requisição não implementada.';
                    continue;
                }
                if ($retorno = $nomeFuncao($_POST)) {
                    $msgAlerta = 'Sua requisição foi executada com sucesso.';
                } else {
                    $msgAlerta = 'Não foi possível executar sua requisição.';
                }
                break;
            case 'json':
                $msg = '';
                header('Content-Type: application/json; charset=ISO-8859-1');
                // -- Função não implementada
                if (!is_callable($nomeFuncao)) {
                    die(simec_json_encode(array('error' => 'Requisição não implementada.')));
                }
                $retorno = $nomeFuncao($_POST);
                die(simec_json_encode(array('error' => $msg, 'options' => $retorno)));
            case 'jsonResponsavelTempoExecucao':
                header('Content-Type: application/json; charset=ISO-8859-1');
                $retorno = jsonResponsavelTempoExecucao($_POST);
                die(simec_json_encode($retorno));
                exit;
            case 'jsonUpdateEntregavel':
                header('Content-Type: application/json; charset=ISO-8859-1');
                $retorno = jsonUpdateEntregavel($_POST);
                die(simec_json_encode($retorno));
                exit;
            case 'filtrar':
                // -- Função não implementada
                if (!is_callable($nomeFuncao)) {
                    $msgAlerta = "Requisição não implementada ({$nomeFuncao}).";
                    continue;
                }
                $msgAlerta = false;
                $queryParams = $nomeFuncao($_POST);
                break;
            case 'voltar':
                $msgAlerta = false;
                break;
            default:
                $msgAlerta = 'Sua requisição é inválida.';
        }
        if (('carregar' != $_POST['action'])) {
    ?>
<script type="text/javascript" language="javascript">
<?php if ($msgAlerta): ?>
alert('<?php echo $msgAlerta; ?>');
<?php endif;?>
window.location = '<?php echo $_SERVER['REQUEST_URI'] . $queryParams; ?>';
</script>
    <?php
            exit();
        }
    }
    return $retorno;
}

/**
 * Remove parametros de busca da URI base do sistema.
 */
function limpaURI()
{
    $tmpURI = explode('&', $_SERVER['REQUEST_URI']);
    $_SERVER['REQUEST_URI'] = "{$tmpURI[0]}&{$tmpURI[1]}";
}

/**
 * Cria uma string de filtro para anexar à URI
 * @param array $campos
 *      Os campos que devem ser avaliados na listagem de dados.
 * @param array $dados
 *      Dados enviados pelo formulário.
 * @return string
 */
function criaFiltroURI($campos, $dados)
{
    $params = '';
    foreach ($campos as $campo) {
        if (!empty($dados[$campo])) {
            $params .= "&{$campo}={$dados[$campo]}";
        }
    }
    return $params;
}

function retornaSolicitante($cpf) {
    
    global $db;
    
    $sqlPartial = <<<DML
SELECT DISTINCT u.usucpf AS codigo, u.usunome AS descricao
  FROM seguranca.usuario AS u
    INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
    INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
  WHERE us.sisid = 44
    AND us.suscod = 'A'
    AND ur.rpustatus = 'A'
    AND ur.pflcod in (238, 237)
    AND u.usucpf = '%s'
DML;
    
    $strSql = sprintf($sqlPartial, $cpf);
    $return = $db->carregar($strSql);
    return (object)$return[0];
}