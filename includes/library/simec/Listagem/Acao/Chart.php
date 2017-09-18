<?php
/**
 * $Id: View.php rafaelJ $
 */

/**
 *
 */
class Simec_Listagem_Acao_Chart extends Simec_Listagem_Acao
{
    protected $icone = 'fa fa-bar-chart-o';
    protected $titulo = 'Abrir Gráficos';
    protected $cor = 'primary';
    
    protected function renderGlyph()
    {
        $html = '
            <span class="btn btn-%s btn-sm %s"></span>
        ';
        return sprintf($html, $this->cor, $this->icone);
    }
}
