<?php
class Model_Instituicaocampus extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'pdu';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'instituicaocampus';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);
        
        $this->entity['cmpid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['cmpdscrazaosocial'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '255' , 'contraint' => '');
        $this->entity['cmpdscsigla'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['cmpcnpj'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '14' , 'contraint' => '');
        $this->entity['cmpcodunidade'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '6' , 'contraint' => '');
        $this->entity['cmpemail'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['cmpfonecomercial'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['cmpfonefax'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['cmpdsccaracteristica'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpcep'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '8' , 'contraint' => '');
        $this->entity['cmplogradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['cmpcompllogradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['cmpbairrologradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['cmpnumlogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '10' , 'contraint' => '');
        $this->entity['cmpsiteinstitucional'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['cmplatitude'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '12' , 'contraint' => '');
        $this->entity['cmplongitude'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '12' , 'contraint' => '');
        $this->entity['muncodlogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '7' , 'contraint' => 'fk');
        $this->entity['estuflogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '2' , 'contraint' => 'fk');
        $this->entity['cmptipo'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['cmpvincendid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['cmpdtinclusao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');

        $this->entity['cmpdtcriacao'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '7' , 'contraint' => '');
        $this->entity['cmpinicioatv'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '7' , 'contraint' => '');
        $this->entity['cmpsitinauguracao'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['cmpdtinauguracao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['tecid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpsitcampus'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpinstalacoes'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['cmpobrascampus'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmptipocampus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['cmpcaracteristicaund'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpinfadicionais'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');

        $this->entity['cmpareatotal'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpareaconstgeral'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpareaconstlab'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['cmpareaconstsala'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');


        $this->entity['intid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }


    public function isValid()
    {
        parent::isValid();

        $isValid = true;
        if($this->entity['cmpdtcriacao']['value']){
            $cmpdtcriacao = $this->entity['cmpdtcriacao']['value'];
            $arrDtCriacao = explode('/' , $cmpdtcriacao);
            if(reset($arrDtCriacao) > 12){
                $this->error[] = array("name" => 'cmpdtcriacao', "msg" => utf8_encode($this->entity['cmpdtcriacao']['value'] . self::MSG_DATA_INVALIDA));
                $isValid = false;
            }

        }

        if($this->entity['cmpinicioatv']['value']){
            $cmpinicioatv = $this->entity['cmpinicioatv']['value'];
            $arrDtIniciativa = explode('/' , $cmpinicioatv);
            if(reset($arrDtIniciativa) > 12){
                $this->error[] = array("name" => 'cmpinicioatv', "msg" => utf8_encode($this->entity['cmpinicioatv']['value'] . self::MSG_DATA_INVALIDA));
                $isValid = false;
            }
        }

        return $isValid;
    }

}
