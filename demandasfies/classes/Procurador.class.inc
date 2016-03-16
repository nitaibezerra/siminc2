<?php
	
class Procurador extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "demandasfies.procurador";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "proid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'proid' => null, 
									  	'pronome' => null, 
									  	'protelefone' => null, 
									  	'proemail' => null, 
									  	'proobservacao' => null, 
									  	'dmdid' => null, 
									  	'estuf' => null,
									  );

    public function getComboUfs() {
        $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
        $dados = $this->carregar ( $sql );
        $dados = $dados ? $dados : array();
        return $this->getOptions ( $dados, array (
            'prompt' => ' Selecione '
        ), 'estuf' );
    }

    public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null) {
        $html = '';
        $selected = '';

        if (isset ( $htmlOptions ['prompt'] )) {
            $html .= '<option value="">' . strtr ( $htmlOptions ['prompt'], array (
                    '<' => '&lt;',
                    '>' => '&gt;'
                ) ) . "</option>\n";
        }

        if ($dados) {
            foreach ( $dados as $data ) {
                if ($idCampo) {
                    $selected = ($data ['codigo'] === $this->$idCampo ? "selected='true' " : "");
                }
                $html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data ['codigo'] . ">  " . simec_htmlentities ( $data ['descricao'] ) . " </option> ";
            }
        }
        return $html;
    }
}