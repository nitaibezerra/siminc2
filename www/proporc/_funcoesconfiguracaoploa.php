<?php
/**
 * Funções da configuração da PLOA.
 *
 * $Id: _funcoesconfiguracaoploa.php 102352 2015-09-11 14:52:35Z maykelbraz $
 */

/**
 * Imprime o painel de despesas.
 *
 * @param array $dadosdespesa Dados da despesa para exibição no painel de cabeçalho.
 */
function painelDetalheCategoriaDespesa($dadosdespesa)
{
    $htmlPainel = <<<HTML
<table class="table table-striped table-bordered table-hover table-condensed">
    <tbody>
        <tr>
            <td><strong>Grupo de despesa:</strong></td>
            <td>{$dadosdespesa['gdpnome']}</td>
        </tr>
        <tr>
            <td><strong>Coluna de despesa:</strong></td>
            <td>{$dadosdespesa['dspnome']}</td>
        </tr>
    </tbody>
</table>
HTML;
    bootstrapPanel('Informações da coluna de despesa', $htmlPainel, 'info', array('cols' => 6));
}

/**
 *
 * @param Proporc_Model_Periodoreferencia $prfref
 * @todo Trocar por um seletor de período
 */
function painelInformacoesPLOA(Proporc_Model_Periodoreferencia $prfref, array $opcoes = array())
{
    $panel = <<<HTML
<table class="table table-bordered">
    <tbody>
        <tr>
            <td class="label-td">Período:</td>
            <td>{$prfref}</td>
        </tr>
HTML;
    foreach ($opcoes as $opcao => $_) {
        switch ($opcao) {
            case 'preenchimentoLimites':
                $limites = new Proporc_Model_Limitesfonteunidadeorcamentaria();
                $dados = $limites->carregarResumoGrupo($prfref->prfid);
                $pbMontante = callbackProgressBar($dados['vlrmontante'], $dados['vlrlimite'], 'barra-montante');
                $pbLimite = callbackProgressBar($dados['vlrlimite'], $dados['vlrdetalhado'], 'barra-detalhamento');
                $panel .= <<<HTML
        <tr>
            <td class="label-td">Limite detalhado:</td>
            <td>{$pbMontante}</td>
        </tr>
        <tr>
            <td class="label-td">Despesa detalhado:</td>
            <td>{$pbLimite}</td>
        </tr>
HTML;
                break;
            default:
                ver($opcao, d);
        }
    }

    $panel .= <<<HTML
    </tbody>
</table>
HTML;
    bootstrapPanel('Informações do período', $panel);
}

function callbackProgressBar($total, $parcial, $id = '', $montante = -1)
{
    if ((0.00 === (double)$total) || (0.00 === (double)$montante)) {
        return <<<HTML
<p style="text-align:center"><span class="label label-danger">Montante não definido</span></p>
HTML;
    }

    $porcentagem = 0;
    if (0.00 !== (double)$parcial) {
        $porcentagem = round((double)$parcial * 100 / (double)$total, 2);
    }

    $porcentagem_tamanho = $porcentagem;
    $porcentagem_cor = 'success';
    if ($porcentagem > 100) {
        $porcentagem_tamanho = 100;
        $porcentagem_cor = 'warning';
    }

    return <<<HTML
    <div class="progress" style="margin-bottom:0px">
            <div class="progress-bar progress-bar-striped progress-bar-{$porcentagem_cor}" role="progressbar"
                 aria-valuenow="{$porcentagem}" aria-valuemin="0" aria-valuemax="100" id="{$id}"
                 style="width:{$porcentagem_tamanho}%;min-width:2em">
                <strong>{$porcentagem}%</strong>
            </div>
        </div>
HTML;
}
