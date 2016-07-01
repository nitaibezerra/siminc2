<?php
/**
 * $Id: Plus.php 97549 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Plus extends Simec_Listagem_AcaoComID
{
    protected $icone = 'plus';
    protected $titulo = 'Detalhar';
    protected $cor = 'primary';

    protected function renderAcao()
    {
        $acao = <<<HTML
<a href="javascript:expandirLinha('%s', %s, '%s');" title="%s" id="arow-%s">%s</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(true),
            $this->getAcaoID(),
            $this->titulo,
            $this->getAcaoID(),
            $this->renderGlyph()
        );
    }
}
