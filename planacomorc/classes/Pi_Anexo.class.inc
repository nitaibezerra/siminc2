<?php

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Classe de abstração da tabela monitora.pi_anexo.
 */
class Pi_Anexo extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "planacomorc.pi_anexo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "piaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'piaid' => null,
        'arqid' => null,
        'pliid' => null,
        'piastatus' => null,
    );

    public function recuperarPorPlanoInterno($pliid)
    {
        $sql = "
            SELECT
                a.arqid,
                a.arqnome || '.' || a.arqextensao AS arqnome,
                a.arqdescricao
            FROM planacomorc.pi_anexo pa
                JOIN public.arquivo a ON(pa.arqid = a.arqid)-- SELECT * FROM public.arquivo ORDER BY 1 DESC
            WHERE
                pa.piastatus = 'A'
                AND pa.pliid = ". (int)$pliid;

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
}