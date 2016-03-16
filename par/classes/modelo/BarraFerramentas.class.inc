<?php
	
class BarraFerramentas extends Modelo{

	public function __construct(){
		
		parent::__construct();

	}
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.barraferramenta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "brfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'brfid' => null, 
									  	'sisid' => null, 
									  	'mnuid' => null,
									  	'usucpf' => null,
									  	'brfstatus' => null
									  );
		
    public function resgataListaDeTodosIconesDaAba( $abacod )
    {
        $sql = "SELECT 
                mnuimagem as url,
                mnudsc as titulo,
                'ferramenta' as class,
                mnulink as id,
                abm.mnuid
            FROM 
                seguranca.aba_menu abm
            INNER JOIN seguranca.menu mnu ON mnu.mnuid = abm.mnuid
            WHERE 
                abm.abacod = '$abacod'
            ORDER BY
                mnudsc";
        return $this->carregar($sql);
    }

	public function resgataListaDeIconesDaAba( $usucpf, $abacod, $sisid )
    {
        $sql = "SELECT 
                mnuimagem as url,
                mnudsc as titulo,
                'ferramenta' as class,
                mnulink as id,
                abm.mnuid,
                bf.usucpf,
                bf.sisid
            FROM 
                seguranca.aba_menu abm
            INNER JOIN seguranca.menu mnu ON mnu.mnuid = abm.mnuid
            INNER JOIN {$this->stNomeTabela} bf ON 
                bf.usucpf = '{$usucpf}' 
                AND bf.sisid = '{$sisid}'
                AND bf.mnuid = mnu.mnuid
            INNER JOIN seguranca.menus_barraferramenta mbf ON mbf.brfid = bf.brfid
            WHERE 
                abm.abacod = '$abacod'
                AND bf.brfstatus = 'A'
            ORDER BY
                mbf.ordem, 
                mnu.mnudsc";
        //ver($sql,d);
        return $this->carregar($sql);
    }

    public function salvarOrdemLista( $params )
    {
        $resposta = true;

        for( $i = 0; $i < count($params); $i++ ){
            $sql = " UPDATE seguranca.menus_barraferramenta
                    SET ordem = {$i}
                    WHERE 
                        brfid = (
                            SELECT brfid
                            FROM {$this->stNomeTabela}
                            WHERE 
                                mnuid = '{$params[$i]['mnuid']}'
                                AND sisid = '{$params[$i]['sisid']}'
                                AND usucpf = '{$params[$i]['usucpf']}'
                        ) ";
            $this->executar( $sql );

            if( !$this->commit() )
                $resposta = false;
        }

        if( $resposta == false )
            echo "er";
        else
            echo "ok";
        exit;
    }

    public function salvarListaSelecionada( $params )
    {
        // acao para seguranca.barraferramenta
        $sql = "
            SELECT COUNT(*) as total
            FROM {$this->stNomeTabela}
            WHERE 
                mnuid = '{$params['mnuid']}'
                AND usucpf = '{$params['usucpf']}'
                AND sisid = '{$params['sisid']}'
        ";
        $resultado = $this->pegaUm( $sql );
        if( $resultado['total'] > 0 ){
            $sql = "
                UPDATE {$this->stNomeTabela}
                SET brfstatus = 'A'
                WHERE 
                    mnuid = '{$params['mnuid']}'
                    AND usucpf = '{$params['usucpf']}'
                    AND sisid = '{$params['sisid']}'
            ";
        }else{
            $sql = "
                INSERT INTO {$this->stNomeTabela} ( mnuid, usucpf, sisid )
                VALUES ( '{$params['mnuid']}', '{$params['usucpf']}', '{$params['sisid']}' )
            ";
        }
        $this->executar( $sql );
        if( !$this->commit() ){
            echo "er";
            exit;
        }

        // acao para seguranca.menus_barraferramenta
        $sql = "
            SELECT *
            FROM {$this->stNomeTabela}
            WHERE 
                mnuid = '{$params['mnuid']}'
                AND usucpf = '{$params['usucpf']}'
                AND sisid = '{$params['sisid']}'
        ";
        $barraferramenta = $this->pegaUm( $sql );
        $sql = "
            SELECT COUNT(*) as total
            FROM seguranca.menus_barraferramenta
            WHERE 
                brfid = '{$barraferramenta['brfid']}'
        ";
        $resultado = $this->pegaUm( $sql );
        if( $resultado['total'] > 0 ){
            $sql = "
                UPDATE seguranca.menus_barraferramenta
                SET ordem = '{$params['posicao']}'
                WHERE 
                    brfid = '{$barraferramenta['brfid']}'
            ";
        }else{
            $sql = "
                INSERT INTO seguranca.menus_barraferramenta ( brfid, mnuid, ordem )
                VALUES ( '{$barraferramenta['brfid']}', '{$params['mnuid']}', '{$params['posicao']}' )
            ";
        }
        $this->executar( $sql );

        if( $this->commit() )
            echo "ok";
        else
            echo "er";
        exit;
    }

    public function retiraDaListaSelecionada( $params )
    {
        $sql = "
            SELECT COUNT(*) as total
            FROM {$this->stNomeTabela}
            WHERE 
                mnuid = '{$params['mnuid']}'
                AND usucpf = '{$params['usucpf']}'
                AND sisid = '{$params['sisid']}'
        ";
        $resultado = $this->pegaUm( $sql );

        if( $resultado['total'] > 0 ){
            $sql = "
                UPDATE {$this->stNomeTabela}
                SET brfstatus = 'I'
                WHERE 
                    mnuid = '{$params['mnuid']}'
                    AND usucpf = '{$params['usucpf']}'
                    AND sisid = '{$params['sisid']}'
            ";
            $this->executar( $sql );
            if( $this->commit() ){
                echo "ok";
                exit;
            }else{
                echo "er";
                exit;
            }
        }else{
            echo "ok";
            exit;
        }
    }
	
}