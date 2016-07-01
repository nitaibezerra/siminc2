<?php
/**
 * $Id: AcaoComID.php 97549 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 *
 */
abstract class Simec_Listagem_AcaoComID extends Simec_Listagem_Acao
{
    protected function renderAcao()
    {
        $acao = <<<HTML
<a href="javascript:%s(%s);" title="%s" id="arow-%s">%s</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(),
            $this->titulo,
            $this->getAcaoID(),
            $this->renderGlyph()
        );
    }
}
