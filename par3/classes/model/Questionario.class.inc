<?php
/**
 * Par_Model_Questionario
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 21-09-2015
 * @link     no link
 */
class Par3_Model_Questionario extends Modelo
{

    /**
     * Nome da tabela especificada
     *
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par.parquestionario';

    /**
     * Chave primaria.
     *
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pqtid');

    /**
     * Atributos
     *
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'pqtid'       		=> null,
        'inuid'       		=> null,
        'qrpid'       		=> null,
        'pqtanoreferencia'  => null,
	);

    /**
     * Funчуo pegarSQLLista
     * - monta SQL para lista de intstrumentos unidade.
     *
     * @return retorna a query.
     *
     */
    public function pegarSQLLista()
    {
        $where = self::prepararFiltro();

        $sql = 'SELECT
    	           inuid,
    	           inudescricao
    	       FROM
    	           par3.instrumentounidade
    	       WHERE
    	           '.implode(' AND ', $where).'
    	       ORDER BY
    	           2';

        return $sql;

    }//end pegarSQLLista()

    public function pegaQuestionario($inuid, $queid)
    {
	    $sql = "SELECT qp.qrpid
	    		  FROM par.parquestionario qp
	    	INNER JOIN questionario.questionarioresposta qr ON qr.qrpid = qp.qrpid
	    		 WHERE qp.inuid = {$inuid}
	    		   AND qr.queid = {$queid}";

    	return $this->pegaUm($sql);
    }//end pegaQuestionario()

}//end class

?>