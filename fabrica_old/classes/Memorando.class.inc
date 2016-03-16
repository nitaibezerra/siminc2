<?php

/**
 * Memorando
 * @author rayner
 *
 */
class Memorando
{

    const TEXTO_PADRAO_MEMORANDO_SQUADRA = '<div style="font-size:12pt;"><p>Ao, </p>
<p>Coordenador de Recursos e Tecnologia da Informação - CRTI </p>
<p>&nbsp;</p>
<p>
	Assunto:
	<strong>Encaminhamento de ordens de serviço para pagamento </strong>
</p>
<p>&nbsp;</p>
<ol>
	<li>
		Solicitamos providências referentes aos faturamentos das ordens de serviços listadas na tabela anexo, a empresa SQUADRA TECNOLOGIA EM SOFTWARE LTDA. Como sustentação destas, encaminhamos anexos os Termos de Solicitação de Serviço, Termos de Recebimento Provisório da Ordem de Serviço e os relatórios de Contagem Detalhada.
	</li>
	<li>
		Informamos ainda que como descrito no Termo de Referência da Fábrica de Software, citamos na tabela anexa, as porcentagens das disciplinas e fases contratadas que correspondem aos pontos por função correspondentes à contagem detalhada das ordens de serviço.
	</li>
	<li>
		Por fim, coloco-me à disposição para os esclarecimentos adicionais eventualmente necessários.
	</li>
</ol>
<blockquote>
	<p>Atenciosamente, </p>
</blockquote>
<p>&nbsp;</p></div>';
    const TEXTO_PADRAO_MEMORANDO_POLITEC = '<div style="font-size:12pt;"><p>Ao, </p>
<p>Coordenador de Recursos e Tecnologia da Informação - CRTI </p>
<p>&nbsp;</p>
<p>
	Assunto:
	<strong>Encaminhamento de ordens de serviço para pagamento </strong>
</p>
<p>&nbsp;</p>
<ol>
	<li>
		Solicitamos providências referentes aos faturamentos das ordens de serviços listadas na tabela anexo, a empresa POLITEC TECNOLOGIA DA INFORMAÇÃO SA. Como sustentação destas, encaminhamos anexos os Termos de Solicitação de Serviço, Termos de Recebimento Provisório da Ordem de Serviço e os relatórios de Contagem Detalhada.
	</li>
	<li>
		Informamos ainda que como descrito no Termo de Referência da Fábrica de Software, citamos na tabela anexa, as porcentagens das disciplinas e fases contratadas que correspondem aos pontos por função correspondentes à contagem detalhada das ordens de serviço.
	</li>
	<li>
		Por fim, coloco-me à disposição para os esclarecimentos adicionais eventualmente necessários.
	</li>
</ol>
<blockquote>
	<p>Atenciosamente, </p>
</blockquote>
<p>&nbsp;</p></div>';
    const TEXTO_PADRAO_MEMORANDO_EFICACIA = '<div style="font-size:12pt;"><p>Ao, </p>
<p>Coordenador de Recursos e Tecnologia da Informação - CRTI </p>
<p>&nbsp;</p>
<p>
	Assunto:
	<strong>Encaminhamento de ordens de serviço para pagamento </strong>
</p>
<p>&nbsp;</p>
<ol>
	<li>
		Solicitamos providências referentes aos faturamentos das ordens de serviços listadas na tabela anexo, a empresa EFICÁCIA ORGANIZAÇÃO LTDA ME. Como sustentação destas, encaminhamos anexos os relatórios de contagens e os Termos de Abertura da Ordem de serviço e Recebimento Provisório.
	</li>
	<li>
		Por fim, coloco-me à disposição para os esclarecimentos adicionais eventualmente necessários.
	</li>
</ol>
<blockquote>
	<p>Atenciosamente, </p>
</blockquote>
<p>&nbsp;</p></div>';
    const TEXTO_PADRAO_MEMORANDO_SAA = '<div style="font-size:12pt;"><p>Ao, </p>
<p>Coordenador de Recursos e Tecnologia da Informação - CRTI </p>
<p>&nbsp;</p>
<p>
	Assunto:
	<strong>Encaminhamento de ordens de serviço para pagamento </strong>
</p>
<p>&nbsp;</p>
<ol>
	<li>
		Solicitamos providências referentes aos faturamentos das ordens de serviços listadas na tabela anexo, a empresa SUBSECRETARIA DE ASSUNTOS ADMINISTRATIVOS. Como sustentação destas, encaminhamos anexos os relatórios de contagens e os Termos de Abertura da Ordem de serviço e Recebimento Provisório.
	</li>
	<li>
		Por fim, coloco-me à disposição para os esclarecimentos adicionais eventualmente necessários.
	</li>
</ol>
<blockquote>
	<p>Atenciosamente, </p>
</blockquote>
<p>&nbsp;</p></div>';

    private $id;
    private $numeroMemorando;
    private $fiscal;
    private $dataMemorando;
    private $statusMemorando;
    private $prestadorServico;
    private $textoMemorando;
    private $listaDeOrdensDeServicos;
    private $glosaMemorando;
    private $justificativaGlosaMemorando;
    private $descricaoAjuste;
    private $valorAjuste;
    private $tpdpsid;

    public function setTipoDespesaId( $id )
    {
        $this->tpdpsid = $id;
    }

    public function getTipoDespesaId()
    {
        return $this->tpdpsid;
    }
    
    public function setId( $id )
    {
    	$this->id = $id;
    }
    
    public function getId()
    {
    	return $this->id;
    }

    public function setNumeroMemorando( $numeroMemorando )
    {
        $this->numeroMemorando = $numeroMemorando;
    }

    public function getNumeroMemorando()
    {
        return $this->numeroMemorando;
    }

    public function setFiscal( $fiscal )
    {
        $this->fiscal = $fiscal;
    }

    public function getFiscal()
    {
        return $this->fiscal;
    }

    public function setGlosaMemorando( $glosaMemorando )
    {
        $this->glosaMemorando = $glosaMemorando;
    }

    public function getGlosaMemorando()
    {
        return $this->glosaMemorando > 0 ? $this->glosaMemorando : NULL;
    }

    public function setJustificativaGlosaMemorando( $justificativaGlosaMemorando )
    {
        $this->justificativaGlosaMemorando = $justificativaGlosaMemorando;
    }

    public function getJustificativaGlosaMemorando()
    {
        return $this->justificativaGlosaMemorando;
    }

    public function setDescricaoAjuste( $descricaoAjuste )
    {
    	$this->descricaoAjuste = $descricaoAjuste;
    }
    
    public function getDescricaoAjuste()
    {
    	return $this->descricaoAjuste;
    }
    public function setValorAjuste( $valorAjuste )
    {
    	$this->valorAjuste = $valorAjuste;
    }
    
    public function getValorAjuste()
    {
    	return $this->valorAjuste;
    }

    public function setDataMemorando( $dataMemorando )
    {
        $this->dataMemorando = $dataMemorando;
    }

    public function getDataMemorando()
    {
        return $this->dataMemorando;
    }

    public function getDataMemorandoFormatada()
    {
        $mesesEmPortugues = array( "janeiro", "fevereiro", "mar&ccedil;o", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro" );
        $mesPorExtenso = $mesesEmPortugues[$this->getDataMemorando()->format( "m" ) - 1];
        return "Em " . $this->getDataMemorando()->format( "d" ) . " de " . $mesPorExtenso . " de " . $this->getDataMemorando()->format( "Y" );
    }

    public function setPrestadorServico( $prestadorServico )
    {
        $this->prestadorServico = $prestadorServico;
    }

    public function getPrestadorServicoMemorando()
    {
        return $this->prestadorServico;
    }

    public function setTextoMemorando( $textoMemorando )
    {
        $this->textoMemorando = $textoMemorando;
    }

    public function getTextoMemorando()
    {
        return $this->textoMemorando;
    }

//	public function getTextoMemorandoSemHtml(){
//		$textoMemorando = str_replace("</p>", "\n", $this->textoMemorando);
//		$textoMemorando = str_replace('<p class="textoIdentado">', "\t", $textoMemorando);
//		return $textoMemorando;
//	}

    public function setListaDeOrdensDeServico( $ordemDeServico )
    {
        $this->listaDeOrdensDeServicos = $ordemDeServico;
    }

    public function getListaDeOrdensDeServico()
    {
        return $this->listaDeOrdensDeServicos;
    }

    public function setStatusMemorando( $statusMemorando )
    {
        $this->statusMemorando = $statusMemorando;
    }

    public function getStatusMemorando()
    {
        return $this->statusMemorando;
    }
    
    public function possuiGlosa()
    {
        return !empty($this->glosaMemorando);
    }
}
