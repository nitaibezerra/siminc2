<?php

/**
 * PrestadorServico
 * @author rayner
 *
 */
class PrestadorServico extends Modelo
{

    CONST PRESTADORA_SERVICO_AUDITORA = 657077;
    const PRESTADORA_SERVICO_FABRICA = 657076;
    const PRESTADORA_SERVICO_POLITEC = 796705;
    const PRESTADORA_SERVICO_SAA = 742798;
    const PRESTADORA_SERVICO_MBA = 872127;
    
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.entidade";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "entid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'entid'           => null,
        'entnumcpfcnpj'   => null,
        'entnome'         => null,
        'entemail'        => null,
        'entstatus'       => null,
        'entdatainclusao' => null
    );
    protected $stCampos = "entid, entnumcpfcnpj, entnome, entemail, entstatus, entdatainclusao";

    public function setId( $id )
    {
        $this->arAtributos['entid'] = $id;
    }

    public function getId()
    {
        return $this->arAtributos['entid'];
    }

    public function setCpfCnpj( $cpfCnpj )
    {
        $this->arAtributos['entnumcpfcnpj'] = $cpfCnpj;
    }

    public function getCpfCnpj()
    {
        return $this->arAtributos['entnumcpfcnpj'];
    }

    public function setNome( $nome )
    {
        $this->arAtributos['entnome'] = $nome;
    }

    public function getNome()
    {
        return $this->arAtributos['entnome'];
    }

    public function setEmail( $email )
    {
        $this->arAtributos['entemail'] = $email;
    }

    public function getEmail()
    {
        return $this->arAtributos['entemail'];
    }

    public function setStatus( $status )
    {
        $this->arAtributos['entstatus'] = $status;
    }

    public function getStatus()
    {
        return $this->arAtributos['entstatus'];
    }

    public function setDataInclusao( $dataInclusao )
    {
        $this->arAtributos['entdatainclusao'] = new DateTime( $dataInclusao );
    }

    public function getDataInclusao()
    {
        return new DateTime( $this->arAtributos['entdatainclusao'] );
    }

    public function recuperePorCpfCnpj( $cpfCnpj )
    {
        $arClausulas = array( "entstatus = 'A'", "entnumcpfcnpj = '$cpfCnpj'" );
        $stOrdem   = "entdatainclusao DESC LIMIT 1";
        $resultSet = parent::recuperarTodos( $this->stCampos, $arClausulas, $stOrdem );
        foreach ( $resultSet as $linha )
        {
            $p = new PrestadorServico();
            $p->setId( $linha['entid'] );
            $p->setCpfCnpj( $linha['entnumcpfcnpj'] );
            $p->setNome( $linha['entnome'] );
            $p->setEmail( $linha['entemail'] );
            $p->setStatus( $linha['entstatus'] );
            $p->setDataInclusao( $linha['entdatainclusao'] );
        }
        return $p;
    }

    public function recuperePorId( $id )
    {
        $arClausulas = array( "entstatus = 'A'", "entid = '$id'" );
        $resultSet = parent::recuperarTodos( $this->stCampos, $arClausulas );
        foreach ( $resultSet as $linha )
        {
            $p = new PrestadorServico();
            $p->setId( $linha['entid'] );
            $p->setCpfCnpj( $linha['entnumcpfcnpj'] );
            $p->setNome( $linha['entnome'] );
            $p->setEmail( $linha['entemail'] );
            $p->setStatus( $linha['entstatus'] );
            $p->setDataInclusao( $linha['entdatainclusao'] );
        }
        return $p;
    }

    public function recupereSquadraEEficacia()
    {
        $prestadoresServico[] = $this->recuperePorCpfCnpj( '' );
        $prestadoresServico[] = $this->recuperePorCpfCnpj( '65620000140' );
        $prestadoresServico[] = $this->recuperePorCpfCnpj( '' );
        $prestadoresServico[] = $this->recuperePorCpfCnpj( '' );
        $prestadoresServico[] = $this->recuperePorCpfCnpj( '' );
        return $prestadoresServico;
    }

}