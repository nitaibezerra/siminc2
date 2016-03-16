<?php
/**
 * $Id: Checkbox.php 81167 2014-06-04 12:24:26Z maykelbraz $
 */

/**
 * 
 */
class Simec_Listagem_Acao_Checkbox extends Simec_Listagem_Acao
{
    public function __construct()
    {
    }

    protected function renderAcao()
    {
        $acao = <<<HTML
<div class="make-switch switch-mini" data-on-label="X" data-off-label="-" data-off="danger">
    <input type="checkbox" name="data[id][%s]" checked />
</div>
HTML;
        return sprintf($acao, $this->getAcaoID());
    }
}