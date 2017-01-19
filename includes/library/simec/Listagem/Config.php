<?php
/**
 * Arquivo de implementação da classe de configurações da Listagem.
 *
 * $Id: Config.php 103935 2015-10-21 16:54:47Z maykelbraz $
 * @filesource
 */

/**
 *
 */
require_once dirname(__FILE__) . '/../Listagem.php';

/**
 *
 *
 * @package Simec\View\Listagem
 */
class Simec_Listagem_Config
{
    /**
     * @var string Id do relatório.
     */
    protected $id = 'tb_render';

    /**
     * @var mixed[] Lista de ações exibidas no relatório.
     */
    protected $acoes = array();
    /**
     * @var mixed[] Lista de condições para exibição das ações no relatório.
     */
    protected $acoesCondicionais = array();
    /**
     * @var mixed[] Lista de campos de agrupamento das ações no relatório.
     */
    protected $acoesAgrupadas = array();

    /**
     * @var string[] Lista de colunas que não são exibidas na listagem.
     */
    protected $colunasOcultas = array();
    /**
     * @var string[] Lista de callbacks de criação de colunas virtuais.
     */
    protected $colunasVirtuais = array();
    /**
     * @var string[] Lista com o nome das colunas da listagem, utilizado em conjunto com Simec_Listagem::SEM_RETORNO_LISTA_VAZIA.
     * @see \Simec_Listagem::SEM_RETORNO_LISTA_VAZIA
     */
    protected $listaColunas = array();

    /**
     * @var int Tipo de totalizador da listagem.
     *
     * @uses Simec_Listagem::TOTAL_SOMATORIO_COLUNA;
     * @uses Simec_Listagem::TOTAL_QTD_REGISTROS;
     */
    protected $totalizador;

    /**
     * @var string[] Lista de colunas totalizadas.
     */
    protected $colunasTotalizadas = array();

    /**
     * @var array Cada elemento é uma callback para um campo. Toda vez que o campo for ser impresso na listagem, ele
     * primeiro é processado pela callback registrada e o resultado do processamento é impresso no lugar do valor do
     * campo. Ex: $callbacksDeCampo = array('nome_do_campo' => nome_da_callback);
     */
    protected $callbacksDeCampo = array();

    /**
     * @var array Cada elemento define uma regra de formatação de linha. As regras são compostas pelos seguintes
     * elementos: $regrasDeLinha = array(
     *     array('campo' => nome_do_campo, 'op' => igual|diferente|contem, 'valor' => valor_para_comparacao_com_campo, 'classe' => nome_da_classe_css_de_modificacao),
     * );
     */
    protected $regrasDeLinha = array();

    /**
     * @var string|mixed Conjunto de dados da lista.
     */
    protected $datasource;

    /**
     * @var string[] Armazena mensagens de validação de callbacks.
     */
    protected $mensagensCallback = array();

    public function salvar($namespace)
    {
        $_SESSION['simec-listagem'][$namespace] = serialize($this);
        return $this;
    }

    public static function carregar($namespace)
    {
        return unserialize($_SESSION['simec-listagem'][$namespace]);
    }

    // -- ID -----------------------------------------------------------------------------------------------------------
    /**
     * Define um ID para a listagem.
     *
     * @param string $id Id da listagem
     * @throws Exception Não permite que o ID seja vazio.
     */
    public function setId($id)
    {
        if (empty($id)) {
            throw new Exception('O ID da listagem não pode ser definido como vazio.');
        }

        $this->id = $id;
    }

    /**
     * Retorna o ID da listagem.
     *
     * @return string Id da listagem
     */
    public function getId()
    {
        return $this->id;
    }

    // -- Ações --------------------------------------------------------------------------------------------------------
    /**
     * Retorna a lista de ações adicionadas à listagem.
     *
     * @return mixed[] Lista de ações.
     */
    public function getAcoes()
    {
        return $this->acoes;
    }

    /**
     * Retorna a quantidade de ações adicionadas à listagem.
     *
     * @return int Quantidade de ações.
     */
    public function getNumeroAcoes()
    {
        return count($this->acoes);
    }

    /**
     * Retorna as condições de exibição de uma ação da listagem.
     *
     * @param string $acao Nome de uma ação adicionada à listagem.
     * @return mixed[] Lista de condições de exibição da ação.
     */
    public function getCondicaoAcao($acao)
    {
        return $this->acoesCondicionais[$acao];
    }

    /**
     * Verifica se uma ação é agrupada.
     *
     * @param string $acao Nome de uma ação.
     * @return bool
     */
    public function acaoEhAgrupada($acao)
    {
        return array_key_exists($acao, $this->acoesAgrupadas);
    }

    /**
     * Retorna a configuração de agrupamento de uma ação.
     *
     * @param string $acao Nome de uma ação.
     * @return mixed[]
     */
    public function getAgrupamentoAcao($acao)
    {
        return $this->acoesAgrupadas[$acao];
    }

    /**
     * Adiciona uma nova ação à listagem.
     *
     * @param string $acao Nome da ação.
     * @param mixed[] $config Configurações da ação.
     * @param array|null $condicoes Lista de condições de exibição da ação.
     * @param array|null $agrupamentos Lista de colunas de agrupamento da ação.
     * @return \Simec_Listagem_Config
     *
     * @uses \Simec_Listagem_Config::setAcaoComoCondicional()
     * @uses \Simec_Listagem_Config::setAcaoComoAgrupada()
     */
    public function addAcao($acao, $config, array $condicoes = null, array $agrupamentos = null)
    {
        $this->acoes[$acao] = $config;

        if (!empty($condicoes)) {
            $this->setAcaoComoCondicional($acao, $condicoes);
        }

        if (!empty($agrupamentos)) {
            $this->setAcaoComoAgrupada($acao, $agrupamentos);
        }

        return $this;
    }

    /**
     * Define uma condição de exibição de uma ação.
     *
     * @param string $acao Nome de uma ação.
     * @param mixed[] $condicoes Lista de condições de exibição da ação.
     * @return \Simec_Listagem_Config
     */
    public function setAcaoComoCondicional($acao, array $condicoes)
    {
        if (is_array($acao)) {
            foreach ($acao as $acao_) {
                $this->acoesCondicionais[$acao_] = $condicoes;
            }
        } else {
            $this->acoesCondicionais[$acao] = $condicoes;
        }

        return $this;
    }

    /**
     * Define um conjunto de colunas de agrupamento de uma ação.
     *
     * @param string $acao Nome de uma ação.
     * @param string[] $nomeColunas Lista de nome de campos do conjunto de dados para agrupamento da ação.
     * @return \Simec_Listagem_Config
     */
    public function setAcaoComoAgrupada($acao, array $nomeColunas)
    {
        if (is_array($acao)) {
            foreach ($acao as $acao_) {
                $this->acoesAgrupadas[$acao_] = $nomeColunas;
            }
        } else {
            $this->acoesAgrupadas[$acao] = $nomeColunas;
        }

        return $this;
    }

    // -- Colunas ocultas ----------------------------------------------------------------------------------------------
    /**
     * Indica que uma, ou mais, colunas(s) da query não será(ão) exibida(s).
     *
     * @param string|array $nomeColuna Nome do campo da query que não será exibido na listagem.
     * @return \Simec_Listagem_Config
     */
    public function ocultarColunas($nomeColuna)
    {
        if (!is_array($nomeColuna)) {
            $this->colunasOcultas[] = $nomeColuna;
        } else {
            // -- Recebendo um array de campo para esconder as acolunas
            foreach ($nomeColuna as $coluna) {
                $this->colunasOcultas[] = $coluna;
            }
        }

        return $this;
    }

    /**
     * Retorna a lista de colunas ocultas.
     *
     * @return string[] Lista de colunas ocultas.
     */
    public function getColunasOcultas()
    {
        return $this->colunasOcultas;
    }

    /**
     * Quantidade de colunas ocultas.
     * @return int
     */
    public function getNumeroColunasOcultas()
    {
        return count($this->colunasOcultas);
    }

    /**
     * Indica se uma coluna está na lista de colunas ocultas.
     * @param string $nomeColuna Nome da colunas para verificação.
     * @return bool
     */
    public function colunaEstaOculta($nomeColuna)
    {
        return in_array($nomeColuna, $this->colunasOcultas);
    }

    // -- Colunas virtuais ---------------------------------------------------------------------------------------------
    /**
     * Cria uma lista de colunas virtuais com a função callback de geração da mesma.
     *
     * @param string|function $callback Função de geração do valor da coluna;
     * @return \Simec_Listagem_Config
     */
    public function addColunaVirtual($callback)
    {
        $this->colunasVirtuais[] = $callback;
        return $this;
    }

    /**
     * Retorna a lista de colunas virtuais.
     * @return string[]
     */
    public function getColunasVirtuais()
    {
        return $this->colunasVirtuais;
    }

    // -- Lista de colunas ---------------------------------------------------------------------------------------------
    public function setListaColunas(array $listaColunas)
    {
        $this->listaColunas = $listaColunas;
        return $this;
    }

    public function getListaColunas()
    {
        return $this->listaColunas;
    }

    // -- Totalizadores ------------------------------------------------------------------------------------------------
    /**
     * Configura o tipo de totalizador da listagem, além de definir quais colunas são totalizadas, qdo aplicável.
     *
     * @param int $totalizador Tipo de totalizador que será utilizado (Simec_Listagem::TOTAL_QTD_REGISTROS, Simec_Listagem::TOTAL_SOMATORIO_COLUNAS).
     * @param string|array $colunas Coluna(s) que será(ão) totalizada(s).
     * @return \Simec_Listagem_Config
     * @throws Exception Lançada qdo um tipo inválido de totalizador é informado.
     *
     * @uses \Simec_Listagem::TOTAL_QTD_REGISTROS
     * @uses \Simec_Listagem::TOTAL_SOMATORIO_COLUNA
     */
    public function setTotalizador($totalizador, $colunas = null)
    {
        if ($totalizador != Simec_Listagem::TOTAL_QTD_REGISTROS && $totalizador != Simec_Listagem::TOTAL_SOMATORIO_COLUNA) {
            throw new Exception(
                'Tipo de totalizador inválido. Tipos válidos: Simec_Listagem:TOTAL_QTD_REGISTROS ou Simec_Listagem::TOTAL_SOMATORIO_COLUNA.'
            );
        }

        if (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $totalizador && !is_null($colunas)) {
            $this->totalizarColunas($colunas);
        }

        $this->totalizador = $totalizador;
        return $this;
    }

    public function getTotalizador()
    {
        return $this->totalizador;
    }

    /**
     * Lista de colunas que serão totalizadas.
     *
     * @param string|array $colunas Nome(s) da(s) coluna(s) que serão totalizadas.
     * @return \Simec_Listagem_Config
     */
    public function totalizarColunas($colunas)
    {
        if (empty($colunas)) {
            return $this;
        }

        if (!is_array($colunas)) {
            $this->colunasTotalizadas[] = $colunas;
        } else {
            foreach ($colunas as $coluna) {
                $this->colunasTotalizadas[] = $coluna;
            }
        }
    }

    /**
     * Retorna a lista de colunas totalizadas.
     *
     * @return string[]
     */
    public function getColunasTotalizadas()
    {
        return $this->colunasTotalizadas;
    }

    /**
     * Verifica se uma coluna é totalizada.
     * @param string $nomeColuna Nome da coluna totalizada.
     * @return bool
     */
    public function colunaEhTotalizada($nomeColuna)
    {
        return in_array($nomeColuna, $this->colunasTotalizadas);
    }

    // -- Cabecalho ----------------------------------------------------------------------------------------------------
    /**
     * Lista de títulos das colunas do relatório. Também cria títulos de duas colunas,
     * para isso, passe o nome da coluna principal como chave do array e as colunas filhas como
     * itens deste array.
     * Exemplo cabecalho simples:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Coluna 1', 'Coluna 2'));
     * Exemplo cabecalho de dois níveis:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Grupo de colunas' => array('Coluna 1', 'Coluna 2'));
     * Obs: Se passar a string 'auto', serão utilizados os nomes das colunas presentes
     * no conjunto de dados da listagem.
     *
     * @param array|string $cabecalho Array com o título de cada coluna, ou a palavra 'auto'.
     * @todo Transformar o valor 'auto' no padrão da classe.
     */
    public function setCabecalho($cabecalho)
    {
        if (('auto' != $cabecalho) && !is_array($cabecalho)) {
            throw new Exception("\$cabecalho deve ser um array, ou o valor 'auto'.");
        }
        $this->cabecalho = $cabecalho;
    }

    public function getCabecalho()
    {
        return $this->cabecalho;
    }

    // -- Callbacks de campo -------------------------------------------------------------------------------------------
    /**
     * Adiciona uma função callback de processamento de conteúdo de campo.
     * Uma ação comum que pode ser executada com este método, é a aplicação de máscara em um campo de CPF.
     *
     * Exemplo de utilização:<pre>
     * function mascaraReal($valor)
     * {
     * &nbsp;&nbsp;&nbsp;&nbsp;return "R$ {$valor}";
     * }
     * [...]
     * $listagem = new Simec_Listagem();
     * $listagem->setQuery("SELECT '3.00' AS valor");
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addCallbackDeCampo('valor', 'mascaraReal');
     * $listagem->render();</pre>
     *
     * @param string|array $nomeColuna Nome(s) da(s) colunas(s) que receberá(ão) o tratamento.
     * @param string $nomeCallback Nome da função de processamento do campo. Ela deve retornar sempre uma string.
     * @return \Simec_Listagem_Config
     * @throws Exception Gerada quando o nome da callback ou a própria função é inválida.
     */
    public function addCallbackDeCampo($nomeColuna, $nomeCallback)
    {
        if (empty($nomeColuna)) {
            return;
        }

        if (empty($nomeCallback)) { // -- Foi informado o nome da função?
            throw new Exception('O nome da função de callback do campo nao pode ser vazia.');
        }
        if (!is_callable($nomeCallback)) { // -- A função foi declarada??
            throw new Exception("A função '{$nomeCallback}' não está declarada.");
        }

        // -- Preferencialmente (para funcionamento da exportação xls), as callbacks devem estar declaradas no arquivo
        // -- _funcoes.php do módulo.
        if (is_string($nomeCallback)) {
            $reflection = new ReflectionFunction($nomeCallback);
            $arquivoFuncao = $reflection->getFileName();
            if (!in_array(basename($arquivoFuncao), array('funcoesspo.php', '_funcoes.php'))) {
                $this->mensagensCallback[] = <<<HTML
Para que a exportação automática XLS funcionar corretamente, a função de callback <b>{$nomeCallback}()</b> deve ser declarada no arquivo <b>_funcoes.php</b> deste módulo. Declarada em: <u>{$arquivoFuncao}</u>.
HTML;
            }
//        } else {
//            throw new Exception('A exportação XML não funciona com callbacks anônimas.');
        }

        // -- Recebendo um array de campo para adicionar como callback
        if (!is_array($nomeColuna)) {
            $this->callbacksDeCampo[$nomeColuna] = $nomeCallback;
        } else {
            foreach ($nomeColuna as $campo) {
                $this->callbacksDeCampo[$campo] = $nomeCallback;
            }
        }
        return $this;
    }

    /**
     * Verifica se uma coluna tem uma função de callback associada a ela.
     * @param string $nomeColuna Nome da coluna para verificação.
     * @return bool
     */
    public function colunaTemCallback($nomeColuna)
    {
        return array_key_exists($nomeColuna, $this->callbacksDeCampo);
    }

    /**
     * Retorna o nome da função callback associada a uma coluna.
     * @param string $nomeColuna Nome da coluna.
     * @return string
     */
    public function getCallback($nomeColuna)
    {
        return $this->callbacksDeCampo[$nomeColuna];
    }

    public function getMensagensCallback()
    {
        return $this->mensagensCallback;
    }

    // -- Regras de linha ----------------------------------------------------------------------------------------------
    /**
     * Adiciona uma nova regra de formatação de linha.
     * A nova regra deve atender ao formato armazenado em self::$regrasDeLinha:
     *
     * @param array $regra
     * @return \Simec_Listagem_Config
     *
     * @todo validar a estrutura da nova regra a ser adicionada
     */
    public function addRegraDeLinha(array $regra)
    {
        $this->regrasDeLinha[] = $regra;

        return $this;
    }

    public function getRegrasDeLinha()
    {
        return $this->regrasDeLinha;
    }

    // -- Datasource --------------------------------------------------------------------------------------------------------
    public function setDatasource($datasource)
    {
        $this->datasource = $datasource;
        return $this;
    }

    public function getDatasource()
    {
        return $this->datasource;
    }
}
