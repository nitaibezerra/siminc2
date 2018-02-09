<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Classe de abstração da tabela monitora.pi_anexo.
 */
class Emendas_Model_BeneficiarioAnexo extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emendas.beneficiario_anexo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "beaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'beaid' => null,
        'arqid' => null,
        'benid' => null,
        'beastatus' => null,
    );

    public function buscarPorBeneficiario($benid)
    {
        $sql = "
            SELECT
                a.arqid,
                a.arqnome || '.' || a.arqextensao AS arqnome,
                a.arqdescricao
            FROM emendas.beneficiario_anexo ba
                JOIN public.arquivo a ON(ba.arqid = a.arqid)
            WHERE
                ba.beastatus = 'A'
                AND ba.benid = ". (int)$benid;

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}