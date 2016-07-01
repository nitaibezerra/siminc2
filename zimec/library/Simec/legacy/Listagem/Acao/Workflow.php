<?php
/**
 * $Id: Workflow.php 97549 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 * Ação de exibição da barra do workflow.
 * O nome da função, ao invés de ser utilizado como callback javascript,
 * é utilizado como o parâmetro 'requisicao' e enviado ao servidor, para
 * executar a criação da barra de workflow.
 *
 * Exemplo:
 * <pre>
 * array(2) {
 *   ["requisicao"]=>
 *     string(12) "drawWorkflow"
 *   ["params"]=>
 *     array(2) {
 *       [0]=>
 *         string(5) "70549"
 *       [1]=>
 *         string(8) "30430693"
 *   }
 * }
 * </pre>
 */
class Simec_Listagem_Acao_Workflow extends Simec_Listagem_Acao
{
    protected $icone = 'transfer';
    protected $titulo = 'Situação';
    protected $callbackJS = 'showWorkflow';

    protected function renderAcao()
    {
        // -- Ação avançada
        $acao = <<<HTML
<a href="#" class="workflow" data-action="%s" data-params="%s">%s</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(true),
            $this->renderGlyph()
        );
    }
}
