<?php
/**
 * Service de gestão de limites.
 *
 * $Id: Limites.php 102352 2015-09-11 14:52:35Z maykelbraz $
 * @filesource
 */

/**
 *
 */
class Proporc_Service_Limites extends Spo_Service_Abstract
{
    protected $separadorLinha = '/[\t]/';

    protected $qtdRegistros = array(
        'total' => 0,
        'invalidos' => 0,
        'processados' => 0,
        'confirmacoes' => 0,
        'ignorados' => 0,
    );

    protected $registros = array(
        'invalidos' => array(),
        'confirmacoes' => array(),
        'ignorados' => array(),
    );

    public function detalharCategoriasDoGrupo()
    {
        if (empty($this->gdpid)) {
            throw new Exception('O gdpid deve ser informado.');
        }

        $list = new Simec_Listagem();
        $list->setQuery(
            Proporc_Model_Limitesfonteunidadeorcamentaria::querySomatorioCategorias(array('dsp.gdpid' => $this->gdpid))
        )->addCallbackDeCampo(array(
            'vlrmontante',
            'vlrlimite',
            'vlrdetalhado',
            'saldomontante',
            'saldolimite'), 'mascaraMoeda')
        ->addCallbackDeCampo('dspnome', 'alinharEsquerda')
        ->addAcao('edit', 'detalharLimitesCategoria')
        ->setCabecalho(array('Categoria', 'Montante (R$)', 'Limites (R$)', 'Detalhado (R$)', 'M - L (R$)', 'L - D (R$)'))
        ->setId("categorias-do-grupo-{$this->gdpid}")
        ->setIdLinha('cat')
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function detalharLimitesCategoria()
    {
        $categoria = new Proporc_Model_Despesa();
        $categoria->dspid = $this->dspid;
        $select = 't1.dspnome, t2.gdpnome';
        $where = array("dspid = {$this->dspid}");
        $join = array('join' => 'gdpid');
        $dados = $categoria->recuperarTodos($select, $where, null, $join);
        $dados = !empty($dados)?current($dados):array();

        $html = <<<HTML
<table class="table table-bordered table-condensed">
    <tbody>
        <tr>
            <td class="label-td">Grupo:</td>
            <td>{$dados['gdpnome']}</td>
        </tr>
        <tr>
            <td class="label-td">Categoria:</td>
            <td>{$dados['dspnome']}</td>
        </tr>
    </tbody>
</table>
HTML;
        echo bootstrapPanel('Informações da despesa', $html);

        if (empty($this->dspid)) {
            throw new Exception('O dspid deve ser informado.');
        }

        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $list->setQuery(
            Proporc_Model_Limitesfonteunidadeorcamentaria::queryDetalheCategoriaDespesa($this->dspid)
        )->setCabecalho(array('Unidade Orçamentária', 'Fonte recurso', 'Limite (R$)', 'Detalhado (R$)'))
        ->esconderColunas('dspid', 'unidsc')
        ->addCampo(array('id' => 'detalhe-dspid', 'name' => 'limitecategoria[dspid]', 'type' => 'hidden'))
        ->addCampo(array('id' => 'detalhe-requisicao', 'name' => 'requisicao', 'type' => 'hidden'))
        ->addCallbackDeCampo('unicod', 'formatarUnicod')
        ->addCallbackDeCampo('vlrlimite', 'formatarVlrlimite')
        ->addCallbackDeCampo('vlrdetalhado', 'mascaraMoeda')
        ->turnOnPesquisator()
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function salvarLimitesCategoria(array $dados = array())
    {
        $limites = new Proporc_Model_Limitesfonteunidadeorcamentaria();

        if (empty($this->dspid)) {
            throw new Exception('O dspid deve ser informado.');
        }

        $unicods = $dados['unicod'];
        $foncods = $dados['foncod'];
        $vlrlimites = $dados['vlrlimite'];

        foreach ($unicods as $key => $unicod) {
            $foncod = $foncods[$key];
            $vlrlimite = $vlrlimites[$key];

            if ('' === $vlrlimite) {
                continue;
            }

            $limites->dspid = $this->dspid;
            $limites->unicod = $unicod;
            $limites->foncod = $foncod;
            $limites->vlrlimite = $vlrlimite;
            $limites->salvar();
        }
        $limites->commit();
    }

    public function carregarLimites()
    {
        if (empty($this->dspid)) {
            throw new Exception('O dspid deve ser informado.');
        }

        $limites = new Proporc_Model_Limitesfonteunidadeorcamentaria();
        $limites->dspid = $this->dspid;

        $dados = $limites->carregarLimitesPorCategoria();
        foreach ($dados as &$item) {
            $item['vlrmontante'] = mascaraMoeda($item['vlrmontante']);
            $item['vlrlimite'] = mascaraMoeda($item['vlrlimite']);
            $item['vlrdetalhado'] = mascaraMoeda($item['vlrdetalhado']);
            $item['saldomontante'] = mascaraMoeda($item['saldomontante']);
            $item['saldolimite'] = mascaraMoeda($item['saldolimite']);
        }

        return $dados;
    }

    public function carregarOpcoesCategoria()
    {
        $opcoes = array('join' => 'gdpid');
        $where = array();
        if (!empty($this->gdpid)) {
            $where[] = "t1.gdpid = {$this->gdpid}";
        }
        if (!empty($this->prfid)) {
            $where[] = "t2.prfid = {$this->prfid}";
        }
        $despesas = new Proporc_Model_Despesa();
        $dados = $despesas->recuperarTodosFormatoInput('dspnome', $where, null, $opcoes);

        return $dados;
    }

    public function importarLimites()
    {
        if (empty($this->limites)) {
            throw new Exception('Não foi enviada nenhuma informação de limites.');
        }

        $this->limites = explode(PHP_EOL, $this->limites);
        if (false == count($this->limites)) {
            throw new Exception('Os dados enviados são inválidos. Não foi encontrada nenhuma linha de limites.');
        }

        $this->salvarLimitesImportacao();

        return (bool)$this->qtdRegistros['confirmacoes'];
    }

    protected function processaRetornoImportacao()
    {
        $mensagem = <<<DML
Resultado do processamento:
<ul>
    <li><strong>Total de registros</strong>: {$this->qtdRegistros['total']};</li>
    <li><strong>Registros processados</strong>: {$this->qtdRegistros['processados']};</li>
    <li><strong>Registros ignorados</strong>: {$this->qtdRegistros['ignorados']};</li>
    <li><strong>Registros inválidos</strong>: {$this->qtdRegistros['invalidos']};</li>
    <li><strong>Aguardando confirmação</strong>: {$this->qtdRegistros['confirmacoes']}.</li>
</ul>
DML;
        $this->flashMessage->addMensagem($mensagem);

        // -- Registros inválidos
        if (!empty($this->registros['invalidos'])) {
            $mensagem = <<<DML
Registros inválidos:
<ul>
DML;
            $mensagem .= '<li>' . implode('</li><li>', $this->registros['invalidos']) . '</li>';
            $mensagem .= <<<DML
</ul>
DML;
            $this->flashMessage->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);
        }

        // -- Registros que precisam de confirmação
        $_SESSION['proporc']['importacao-limites'] = null;
        if (!empty($this->registros['confirmacoes'])) {
            foreach ($this->registros['confirmacoes'] as $confirmacao) {
                list($unicod, $foncod, $valor) = preg_split($this->separadorLinha, $confirmacao[0]);
                $_SESSION['proporc']['importacao-limites']['confirmacoes'][] = array(
                    'id' => $confirmacao[0],
                    'unicod' => $unicod,
                    'foncod' => $foncod,
                    'valor' => $confirmacao[1],
                    'novovalor' => str_replace(array('.', ','), array('', '.'), $valor),
                );
            }
        }
    }

    public function substituirLimites()
    {
        if (empty($this->limites)) {
            throw new Exception('Não foi enviada nenhuma informação de limites.');
        }
        if (false == count($this->limites)) {
            throw new Exception('Os dados enviados são inválidos. Não foi encontrada nenhuma linha de limites.');
        }

        $this->salvarLimitesImportacao($confirmar = false);
    }

    protected function salvarLimitesImportacao($confirmar = true)
    {
        $despesaFonte = new Proporc_Model_Despesafonterecurso();
        $mdlLimite = new Proporc_Model_Limitesfonteunidadeorcamentaria();

        foreach ($this->limites as $limite) {
            $this->qtdRegistros['total']++;

            $pedacos = preg_split($this->separadorLinha, $limite);
            if (3 != count($pedacos)) {
                $this->registros['invalidos'][] = "Linha inválida: {$limite}";
                $this->qtdRegistros['invalidos']++;
                continue;
            }

            list($unicod, $foncod, $vlrlimite) = $pedacos;
            $mdlLimite->dspid = $this->dspid;
            $mdlLimite->foncod = $foncod;
            $mdlLimite->unicod = $unicod;
            $mdlLimite->vlrlimite = $vlrlimite;

            // -- Separa para solicitação de configuração
            if ($valor = $mdlLimite->existe()) {
                if ($valor == str_replace(array('.', ','), array('', '.'), $vlrlimite)) {
                    // -- ignorado
                    $this->qtdRegistros['ignorados']++;
                    $this->registros['ignorados'][] = $limite;
                    continue;
                } elseif ($confirmar) {
                    // -- precisa de confirmação
                    $this->qtdRegistros['confirmacoes']++;
                    $this->registros['confirmacoes'][] = array($limite, $valor);
                    continue;
                }
            }

            // -- Se a fonte for válida, insere novo registro
            $despesaFonte->dspid = $this->dspid;
            $despesaFonte->foncod = $foncod;

            if ($despesaFonte->fonteValida()) {
                $mdlLimite->salvar();
                $mdlLimite->clearDados();
                $this->qtdRegistros['processados']++;
                continue;
            }

            // -- Fonte inválida
            $this->qtdRegistros['invalidos']++;
            $this->registros['invalidos'][] = "Fonte não cadastrada para esta coluna (<b>cadastre a fonte na coluna</b>): {$limite}";
        }

        $mdlLimite->commit();
        $this->processaRetornoImportacao();
    }

    public function exportarXLSResumoLimites()
    {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $sql = Proporc_Model_Limitesfonteunidadeorcamentaria::querySomatorioGrupos(array('gdp.prfid' => $this->prfid));

        $listagem->setQuery(
            $sql
        )->addCallbackDeCampo(array(
            'vlrmontante',
            'vlrlimite',
            'vlrdetalhado',
            'saldomontante',
            'saldolimite'), 'mascaraMoeda')
        ->addCallbackDeCampo('dspnome', 'alinharEsquerda')
        ->setCabecalho(array('Nome do grupo', 'Montante (R$)', 'Limites (R$)', 'Detalhado (R$)', 'M - L (R$)', 'L - D (R$)'))
        ->esconderColunas('gdpid')
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
