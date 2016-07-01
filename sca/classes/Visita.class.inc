<?php

include_once APPRAIZ . "sca/classes/Equipamento.class.inc";
include_once APPRAIZ . "sca/classes/CrachaProvisorio.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Visita extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sca.visita";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vstid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array('vstid' => null,
                                       'vttid' => null,
                                       'edaid' => null,
                                       'dstid' => null,
                                       'audid' => null,
                                       'vstnumcracha' => null,
                                       'vstdatentrada' => null,
                                       'vstdatsaida' => null,
                                       'usucpfentrada' => null,
                                       'usucpfsaida' => null,
                                       'vsnumcrachasistema'=>null
                                      );


    public function recuperarVisitante($vttid, $vttdoc){

    	if (!empty($vttid)){
    		$where[] = " t1.vttid = $vttid ";
    	}
    	
		if(!empty($vttdoc)){
            $where[] = " t1.vttdoc = '$vttdoc' ";
        }
        
        if(!empty($where)){
        	$where = "where " . implode("OR", $where);
        }

        $sql = "select t1.vttid, t1.vttdoc, t1.vttnome, t1.vttobs
                      ,t2.vstid, t2.edaid, t2.dstid, t3.edfid, t2.vstdatsaida
                  from sca.visitante t1
                  left join sca.visita t2
                    on t1.vttid = t2.vttid
                  left join sca.edificioandar t3
                    on t2.edaid = t3.edaid
                 $where
                 order by t2.vstid desc
                 limit 1";
        //exit("sql: " . $sql);
        return $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
    }
    
    public function verificaSeVisitanteFoiRegistradoPeloNumeroEtiquetaManual( $vstnumcracha ){
    	$sql = "SELECT count(*)
    			FROM sca.visita v
    			WHERE v.vstnumcracha = $vstnumcracha AND v.vstdatsaida is null";
    	return (bool) $this->pegaUm($sql);
    }
    
    public function verificaSeVisitaEstaAtivaPeloIdVisitante( $idVisitante ) {	
    	$sql = "SELECT count(*) FROM sca.visita WHERE vttid = $idVisitante AND vstdatsaida is null";
    	return (bool) $this->pegaUm($sql);
    }
    
    public function recuperaUltimaVisitaAtivaPeloIdVisitante( $idVisitante ) {
    	$sql = "SELECT * FROM sca.visita WHERE vttid = $idVisitante AND vstdatsaida is null";
    	return $this->pegaLinha($sql);
    }
    
    public function recuperaUltimaVisitaPeloIdVisitante( $idVisitante ) {
    	$sql = "SELECT * FROM sca.visita WHERE vttid = $idVisitante ORDER BY vstid DESC LIMIT 1";
    	return $this->pegaLinha($sql);
    }
    
    public function recuperaUltimaVisitaInserida(){
        $sql = "SELECT currval('sca.visita_vstid_seq') as vstid";
        $result = $this->pegaLinha($sql);

        $visita = $this->recuperarVisitaCompleto( $result['vstid'] );

        $this->arAtributos =$visita; // seta os atributos da visita 
        return $result['vstid'];
    }
    
    public function recuperaAtributos(){
        return $this->arAtributos;
    }
    

    public function recuperarServidor($nu_matricula_siape, $nu_cpf = null, $is_cpf = null){

        if(!empty($nu_matricula_siape) && is_numeric($nu_matricula_siape)){
            $where = "where servidor.nu_matricula_siape = $nu_matricula_siape";
        } elseif(!empty($nu_cpf) && is_numeric($nu_cpf)){

        	if($is_cpf) {
        		$where = "where servidor.nu_cpf = '$nu_cpf'";
        	} else {
        		$where = "where servidor.nu_matricula_siape = $nu_cpf";
        	}
        	
        } else{
            $where = "where 1 = 2";
        }

        $sql = "select servidor.nu_matricula_siape
                      ,servidor.nu_cpf as vttdoc
                      ,servidor.no_servidor as vttnome
                      ,cargo.ds_cargo_emprego as vttobs
                      ,cargo.ds_cargo_emprego
                      ,servidor.no_unidade_org
                      ,servidor.co_interno_uorg
                      ,servidor.no_unidade_org no_uorg
                      ,servidor.co_interno_uorg co_uorg
					  ,usuario.usufonenum
					  ,usuariodetalhes.usdsala
                  from sca.vwservidorativo servidor
         		  left join siape.vwservidorativo servidor_siape on (servidor.nu_cpf = servidor_siape.nu_cpf)
				  left join siape.tb_simec_cargo_emprego cargo on (servidor_siape.co_cargo_emprego = cargo.co_cargo_emprego)
				  left join seguranca.usuario usuario on (usuario.usucpf = servidor.nu_cpf)
				  left join demandas.usuariodetalhes usuariodetalhes on (usuariodetalhes.usucpf = servidor.nu_cpf)
                $where
                 limit 1";
        
        return $this->pegaLinha(iconv("UTF-8", "ISO-8859-1", $sql));
    }
    
    public function recuperarPessoaPorEquipamento($etiqueta) {
    	
    	if ($etiqueta) {
    		
    		$sql = "select v.vttid, s.nu_matricula_siape 
					from sca.equipamento e
					inner join sca.movimentacaoequipamento m on (e.eqmid = m.eqmid)
					left join sca.visitante v on (m.vttid = v.vttid)
					left join sca.vwservidorativo s on (m.nu_matricula_siape = s.nu_matricula_siape)
					where e.eqmnumetiqueta = '$etiqueta' order by m.mveid DESC";
    		
    		return $this->pegaLinha(iconv("UTF-8", "ISO-8859-1", $sql));
    	} else {
    		return false;
    	}
    }

    public function recuperarPessoa($vttid, $vttdoc, $nu_matricula_siape){


        if(empty($vttid)){
            //tenta encontrar um servidor
            $servidor = $this->recuperarServidor($nu_matricula_siape, $vttdoc);

            if($servidor){
                return $servidor;
            }
        }

        return $this->recuperarVisitante($vttid, $vttdoc);
    }

    public function recuperarAndaresEdificio($edfid){

        if($edfid){
            $sql = "select edaid as codigo, edadsc as descricao
                      from sca.edificioandar an
                      join sca.edificio ed
                        on ed.edfid = an.edfid
                     where an.edfid = $edfid
                     order by 2";

            $andares = $this->carregar($sql);
            $andares_array = array();
            if($andares && count($andares) > 0){
                foreach ($andares as $andar){
                    $andares_array[] = array('codigo' => $andar['codigo'], 'descricao' => utf8_encode($andar['descricao']));
                }
            }
            return $andares_array;
        } else{
            return array();
        }
    }

    
    /**
     * Gera numero para Cracha Etiqueta
     */
    public function geraNumeroCrachaSistema(){

        $sql = "
            SELECT 
            	vsnumcrachasistema, 
            	CAST( SUBSTR(vsnumcrachasistema, 5, 10) AS numeric) as numero,
            	SUBSTR(vsnumcrachasistema, 0, 5) as ano
            FROM sca.visita 
            WHERE 
            	vsnumcrachasistema IS NOT NULL
            	AND vsnumcrachasistema LIKE '%'|| date_part('year', now()) ||'%'
            ORDER BY numero DESC LIMIT 1
        	";

        $rsNumero = $this->pegaLinha($sql);        
       
        if($rsNumero && count($rsNumero) > 0){
            // Incrementa Numero
            if($rsNumero['ano']==date('Y')){
                $novo_numero = date('Y') . (str_pad( $rsNumero['numero']+1 , 6, "0", STR_PAD_LEFT)) ;
            }else{
                $novo_numero = $rsNumero['ano']. (str_pad( $rsNumero['numero']+1 , 6, "0", STR_PAD_LEFT));
            }
        }else{
            $novo_numero = date('Y') . str_pad( 1 , 6, "0", STR_PAD_LEFT) ;
        }
        // retorna 
        return $novo_numero;
    }

    /**
     * M?todo respons?vel por montar a combo do edif?cio
     *
     * @name montaComboEdificio
     * @author C?zar Cirqueira
     * @access public
     * @return select com todos edif?cios
     */
    public function montaComboEdificio(){
        $sql = "select edfid as codigo, edfdsc as descricao
                  from sca.edificio
                 order by 2";

        $this->monta_combo("edfid",$sql,'S',"Selecione...","buscarAndarEdificio","","","303","S","edfid","",$edfid);
    }

    public function validarEntradaVisitante($vttid, $vstnumcracha, $edaid, $equipamentos){

        $oEquipamento = new Equipamento();

        //Valida Crachá
        $sql = "select vstnumcracha
                  from sca.visita
                 where vstnumcracha = $vstnumcracha
                   and edaid = $edaid
                   and vstdatsaida is null";

        if(!$this->eof($sql)){
            return "O número do crachá informado está associado a outro visitante neste andar.";
        }

        //Valida visitante
        $sql = "select vttid
                  from sca.visita
                 where vttid = $vttid
                   and vstdatsaida is null";
        if(!$this->eof($sql)){
            return "Não existe registro de saída para a última visita do visitante informado.";
        }

        //valida equipamentos
        for($i = 0; $i < count($equipamentos); $i++){

            $eqmid = $equipamentos[$i][0];
            if(!empty($eqmid)){
                if($oEquipamento->verificaEntradaSemSaida($eqmid)){
                    return "Não existe registro de saída para o equipamento informado.";
                }
            }
        }
        return "";
    }

    public function recuperarVisita($vstid){

        $sql = "select visita.vstid
                      ,coalesce(visitante1.vttid, visitante2.vttid) vttid
                      ,coalesce(visitante1.vttdoc, visitante2.vttdoc, servidor.nu_cpf) vttdoc
                      ,coalesce(visitante1.vttnome, visitante2.vttnome, servidor.no_servidor) vttnome
                      ,coalesce(visitante1.vttobs, visitante2.vttobs) vttobs
                      ,autorizado.nu_matricula_siape
                  from sca.visita visita
                  left join sca.visitante visitante1
                    on visitante1.vttid = visita.vttid
                  left join sca.autorizado autorizado
                    on autorizado.audid = visita.audid
                  left join sca.visitante visitante2
                    on visitante2.vttid = autorizado.vttid
                  left join sca.vwservidorativo servidor
                    on servidor.nu_matricula_siape = autorizado.nu_matricula_siape
                 where visita.vstid = $vstid";

        return $this->carregar($sql);
    }
    
    public function recuperarVisitaCompleto($vstid){
        $sql = "SELECT visita.*
                      ,coalesce(visitante1.vttid, visitante2.vttid) vttid
                      ,coalesce(visitante1.vttdoc, visitante2.vttdoc, servidor.nu_cpf) vttdoc
                      ,coalesce(visitante1.vttnome, visitante2.vttnome, servidor.no_servidor) vttnome
                      ,coalesce(visitante1.vttobs, visitante2.vttobs) vttobs
                      ,autorizado.nu_matricula_siape
                  FROM 
                  	sca.visita visita
                  left join sca.visitante visitante1 on visitante1.vttid = visita.vttid
                  left join sca.autorizado autorizado on autorizado.audid = visita.audid
                  left join sca.visitante visitante2 on visitante2.vttid = autorizado.vttid
                  left join sca.vwservidorativo servidor on servidor.nu_matricula_siape = autorizado.nu_matricula_siape
                 WHERE visita.vstid = $vstid";

        return $this->carregar($sql);
    }

    public function confirmarUsoCracha($nu_matricula_siape, $vstnumcracha, $usucpf, $imagemVisitante){

        $mensagemValidacao = $this->validarEntradaCracha($nu_matricula_siape, $vstnumcracha);

        if(empty($mensagemValidacao)){
            $sql = "insert into sca.crachaprovisorio (nu_matricula_siape, vstnumcracha, usucpfentrada)
                    values ($nu_matricula_siape, $vstnumcracha, '$usucpf') returning cpsid";

            $itemSalvo = $this->pegaUm($sql);

            $servidor = $this->recuperarServidor($nu_matricula_siape);

            //$cracha = new CrachaProvisorio();
            //$cracha->cadastrarCartaoProvisorio($servidor['vttdoc'], $nu_matricula_siape, $vstnumcracha);

            $insertFoto = array("nu_matricula_siape" => $nu_matricula_siape);

            if(!empty($imagemVisitante)){
                $foto = new FilesSimec("visitantefoto", $insertFoto, "sca");
                $foto->setStream("Foto de visitante", $imagemVisitante);
            }

            $this->commit();
            direcionar('?modulo=principal/servidor/registrarCrachaProvisorio&acao=A','Operação realizada com sucesso!');

            return "";

        } else{
            return $mensagemValidacao;
        }
    }

    public function validarEntradaCracha($nu_matricula_siape, $vstnumcracha){

    	if(!is_numeric($vstnumcracha)){
    		return "É necessário informar um número válido para o crachá.";
    	}
    	
        $sql = "select count(*)
                  from sca.crachaprovisorio
                 where vstnumcracha = $vstnumcracha
                   and cpsdatsaida is null";

        $resultado = $this->pegaUm($sql);

        if($resultado > 0){
            return "O Crachá provisório informado está em uso por outro servidor.";
        }

        $sql = "select count(*)
                  from sca.crachaprovisorio
                 where vstnumcracha = $vstnumcracha
                   and cpsdatsaida::date = current_date";

        $resultado = $this->pegaUm($sql);

        if($resultado > 0){
            return "O Crachá provisório informado já foi utilizado hoje por outro servidor.";
        }

        $sql = "select count(t1.*)
                  from sca.crachaprovisorio t1
                 inner join sca.vwservidorativo t2
                    on t1.nu_matricula_siape = t2.nu_matricula_siape
                 where t1.nu_matricula_siape = $nu_matricula_siape
                   and t1.cpsdatsaida::date = current_date";

        $resultado = $this->pegaUm($sql);

        if($resultado > 0){
            return "O servidor informado já utilizou um crachá provisório hoje.";
        }

        $sql = "select count(t1.*)
                  from sca.crachaprovisorio t1
                 inner join sca.vwservidorativo t2
                    on t1.nu_matricula_siape = t2.nu_matricula_siape
                 where t1.nu_matricula_siape = $nu_matricula_siape
                   and t1.cpsdatsaida is null";

        $resultado = $this->pegaUm($sql);

        if($resultado > 0){
            return "O servidor informado já está utilizando um Crachá provisório.";
        }

        return "";
    }

    public function consultarSaidaCracha($vstnumcracha){

        $sql = "select cracha.cpsid
                      ,cracha.vstnumcracha
                      ,servidor.nu_matricula_siape
                      ,servidor.no_servidor as vttnome
                      ,servidor.ds_cargo_emprego
                  from sca.crachaprovisorio cracha
                 inner join sca.vwservidorativo servidor
                    on cracha.nu_matricula_siape = servidor.nu_matricula_siape
                 where cracha.vstnumcracha = $vstnumcracha
                   and cracha.cpsdatsaida is null";

        return $this->pegaLinha($sql);
    }

    public function confirmarBaixaCracha($cpsid, $imagemVisitante){

        $dataAtual = date("Y-m-d H:i").':00';

        $sql = "update sca.crachaprovisorio
                   set cpsdatsaida = '$dataAtual'
                 where cpsid = $cpsid
             returning nu_matricula_siape, vstnumcracha";

        $result = $this->pegaLinha($sql);

        //$cracha = new CrachaProvisorio();
        //$cracha->baixarCartaoProvisorio($result['nu_matricula_siape'], $result['vstnumcracha']);

        if(!empty($imagemVisitante)){
            $insertFoto = array("nu_matricula_siape" => $result['nu_matricula_siape']);

            $foto = new FilesSimec("visitantefoto", $insertFoto, "sca");
            $foto->setStream("Foto de visitante", $imagemVisitante);
        }

        $this->commit();

        direcionar('?modulo=principal/servidor/baixarCrachaProvisorio&acao=A','Operação realizada com sucesso!');
    }
    
    public function consultarPessoaForaHorario($documento=null, $nome=null, $pesquisa=false){
    
    	$resultado = array();
    	
    	// condições de pesquisa
    	if(is_null($documento) && is_null($nome)){
    		$condicao1 ="";
    		$condicao2 ="";
    	}elseif(!is_null($documento) && $documento !='' && !is_null($nome)){
    		$condicao1 =" WHERE vttdoc='$documento' AND vttnome like upper('%$nome%')";
    		$condicao2 =" WHERE nu_cpf='$documento' AND no_servidor like upper('%$nome%')";
    	}elseif(!is_null($documento) && $documento !=''){
    		$condicao1 =" WHERE vttdoc='$documento'";
    		$condicao2 =" WHERE nu_cpf='$documento'";
    	}elseif(!is_null($nome)){
    		$condicao1 =" WHERE vttnome like upper('%$nome%')";
    		$condicao2 =" WHERE no_servidor like upper('%$nome%')";
    	}
    	    	
    	if($pesquisa){
    		// monta lista
    		$sql = "SELECT
    		'<center>
	    		<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || vttdoc || '\');\">
	           		<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
	    		</a>
	         </center>' as acao,
	        vttdoc as documento,
    		vttnome || ' (Terceirizado) ' as descricao
    		FROM
    		sca.visitante vis
    		$condicao1
    		 
    		UNION
    		 
    		SELECT
    		'<center>
	    		<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || nu_cpf || '\');\">
	            	<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
	    		</a>
	         </center>' as acao,
    		nu_cpf as documento,
    		no_servidor || ' (Servidor) ' as descricao
    		FROM
    		sca.vwservidorativo
    		$condicao2";
    		
    		$cabecalho = array("Ação", "Documento", "Nome");
    		
    		$this->monta_lista($sql,$cabecalho,20,30,false,"center");
    		
    	}else{
    		// retorna usuario
    		$sql = "SELECT
    		vttid || '|0' as codigovttid,
    		vttnome || ' (Terceirizado) ' as descricao
    		FROM
    		sca.visitante vis
    		$condicao1
    		 
    		UNION
    		 
    		SELECT
    		'0|' || nu_matricula_siape as codigosiape,
    		no_servidor || ' (Servidor) ' as descricao
    		FROM
    		sca.vwservidorativo
    		$condicao2";  

    		$res = $this->carregar($sql);
    		 
    		$dados['descricao'] = $res[0]['descricao'];
    		$dados['codigovttid'] = $res[0]['codigovttid'];
    		$dados['codigosiape'] = $res[0]['codigosiape'];
    		
    		$resultado = $dados;
    		
    		return $resultado;
    	}
    }
    
    public function verificarPessoaServidor($vttdoc) {
    	if(is_numeric($vttdoc)) {
	    	$sql = "select
	    				count(nu_matricula_siape)
	    			from
	    				sca.vwservidorativo servidor
	    			where
	    				nu_matricula_siape='$vttdoc'";
	
			$result = $this->carregar($sql);
	
	    	if($result[0]['count'] > 0){
	    		return true;
	    	}else{
	    		return false;
	    	}
    	} else {
    		return false;
    	}
    }
    
    public function consultarVisitanteDuplicadoEntrada($vttdoc, $tipo=null){
    	
    	if ($tipo == 'visitante') {
	    	$sql = "select 
	    				vttid,
	    				null as nu_matricula_siape
			    	from 
			    		sca.visitante
			    	where 
			    		vttdoc = '$vttdoc'
			    	order by 
	    				vttid desc";
    	} elseif ($tipo == 'servidor' && is_numeric($vttdoc)) {
    		$sql = "select
    					null as vttid,
		    			nu_matricula_siape
		    		from
		    			sca.vwservidorativo servidor
		    		where
		    			nu_matricula_siape='$vttdoc'";
    		
    	} else {
    		
    		if(!is_numeric($vttdoc)) {
    			$sql = "select
		    				vttid,
		    				null as nu_matricula_siape
		    			
		    			from
		    				sca.visitante
		    			where
		    				vttdoc = '$vttdoc'";
    		} else {
    			$sql = "select
		    				vttid,
		    				null as nu_matricula_siape
		    			
		    			from
		    				sca.visitante
		    			where
		    				vttdoc = '$vttdoc'
		    				
		    			UNION
		    			
		    			select
		    				null as vttid,
		    				nu_matricula_siape
		    			from
		    				sca.vwservidorativo servidor
		    			where
		    				nu_matricula_siape='$vttdoc'";
    		}
    		
    	}
    	
    	return $this->carregar(iconv( "UTF-8", "ISO-8859-1", $sql));
    }
    
    public function listarRegistroDuplicado($vttdoc, $tipo, $saida=false){
    	
    	if ($tipo == 'visitante') {
    		// verifica se esta listando a saida, caso for true, ele busca apenas os registros com saidas nulas
    		if(!$saida) {
    			$sql = "select
    						'<center>
    							<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || v.vttid || '\', null);\">
					    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
					    		</a>
					    	</center>' as acao,
					    	v.vttid||' ' as vttid,
				    		v.vttdoc||' ' as vttdoc,
				    		td.tiddsc||' ' as tiddsc,
    						v.vttnome||' ' as vttnome
	    				from
	    					sca.visitante v
	    				inner join 
	    					sca.tipodocumento td ON v.tidid = td.tidid
	    				where
	    					v.vttdoc = '$vttdoc'
	    				order by
	    					v.vttid desc";
    		} else {
    			$sql = "select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || v.vttid || '\', null);\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			v.vttid||' ' as vttid,
			    			v.vttdoc||' ' as vttdoc,
			    			td.tiddsc||' ' as tiddsc,
			    			v.vttnome||' ' as vttnome
			    		from
			    			sca.visitante v
			    		inner join
			    			sca.tipodocumento td ON v.tidid = td.tidid
			    		inner join
			    			sca.visita vs ON v.vttid = vs.vttid
			    		where
			    			vs.vstdatsaida is null
			    		and
			    			v.vttdoc = '$vttdoc'
			    		order by
			    			v.vttid desc";
    		}
    			$cabecalho = array("Ação", "Codigo Visitante", "Documento","Tipo de Documento", "Nome");
    			
    	} elseif ($tipo == 'servidor') {
    		// verifica se esta listando a saida, caso for true, ele busca apenas os registros com saidas nulas
    		if(!$saida) {
    			$sql = "select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(null, \'' || s.nu_matricula_siape || '\');\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			s.nu_matricula_siape ||' ' as matricula,
			    			s.nu_cpf||' ' as nu_cpf,
			    			'Matrícula SIAPE' as tiddsc,
			    			s.no_servidor||' ' as nome
			    		from
			    			sca.vwservidorativo s
			    		where
			    			s.nu_matricula_siape='$vttdoc'";
    		} else {
    			$sql = "select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(null, \'' || s.nu_matricula_siape || '\');\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			s.nu_matricula_siape ||' ' as matricula,
			    			s.nu_cpf||' ' as nu_cpf,
			    			'Matrícula SIAPE' as tiddsc,
			    			s.no_servidor||' ' as nome
			    		from
			    			sca.vwservidorativo s
			    		inner join
			    			sca.visita vs ON s.nu_matricula_siape = vs.nu_matricula_siape
			    		where
			    			vs.vstdatsaida is null
			    		and
			    			s.nu_matricula_siape='$vttdoc'";
    		}
		    	
		    	$cabecalho = array("Ação", "Matricula Siape","Documento", "Tipo de Documento", "Nome");
    	
    	} else {
    	
    		// verifica se esta listando a saida, caso for true, ele busca apenas os registros com saidas nulas
    		if(!$saida) {
    			$sql = "select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || v.vttid || '\', null);\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			v.vttid||' ' as vttid,
			    			null||' ' as nu_matricula_siape,
			    			v.vttdoc||' ' as vttdoc,
			    			td.tiddsc||' ' as tiddsc,
			    			v.vttnome||' ' as nome
			    		from
			    			sca.visitante v
			    		inner join
			    			sca.tipodocumento td ON v.tidid = td.tidid
			    		where
			    			v.vttdoc = '$vttdoc'
			    			 
			    		UNION
			    			
			    		select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(null, \'' || s.nu_matricula_siape || '\');\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			null||' ' as vttid,
			    			s.nu_matricula_siape||' ' as nu_matricula_siape,
			    			s.nu_cpf||' ' as nu_cpf,
			    			'Matrícula SIAPE' as tiddsc,
			    			s.no_servidor||' ' as nome
			    		from
			    			sca.vwservidorativo s
			    		where
			    			s.nu_matricula_siape='$vttdoc'";
    		} else {
    			$sql = "select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || v.vttid || '\', null);\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			v.vttid||' ' as vttid,
			    			null||' ' as nu_matricula_siape,
			    			v.vttdoc||' ' as vttdoc,
			    			td.tiddsc||' ' as tiddsc,
			    			v.vttnome||' ' as nome
			    		from
			    			sca.visitante v
			    		inner join
			    			sca.tipodocumento td ON v.tidid = td.tidid
			    		inner join
			    			sca.visita vs ON v.vttid = vs.vttid
			    		where
			    			vs.vstdatsaida is null
			    		and
			    			v.vttdoc = '$vttdoc'
			    			 
			    		UNION
			    			
			    		select
			    			'<center>
			    			<a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(null, \'' || s.nu_matricula_siape || '\');\">
			    			<img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
			    			</a>
			    			</center>' as acao,
			    			null||' ' as vttid,
			    			s.nu_matricula_siape||' ' as nu_matricula_siape,
			    			s.nu_cpf||' ' as nu_cpf,
			    			'Matrícula SIAPE' as tiddsc,
			    			s.no_servidor||' ' as nome
			    		from
			    			sca.vwservidorativo s
			    		inner join
			    			sca.visita vs ON s.nu_matricula_siape = vs.nu_matricula_siape
			    		where
			    			vs.vstdatsaida is null
			    		and
			    			s.nu_matricula_siape='$vttdoc'";
    		}
		    	
		    	
		    	$cabecalho = array("Ação", "Codigo Visitante", "Codigo Siape","Documento", "Tipo de Documento", "Nome");
    	}
   
    	$this->monta_lista($sql,$cabecalho,20,30,false,"center");
    }

    public function consultarVisitanteEntrada($vttid, $vttdoc, $nu_matricula_siape, $tipo, $audid = null){

        $resultado = array();

        if($tipo == 'A'){
            if (!empty($audid)){
                $sql = "select vttid
                              ,nu_matricula_siape
                          from sca.autorizado
                         where audid = $audid";

                $autorizacao = $this->pegaLinha($sql);
                $vttid = $autorizacao['vttid'];
                $nu_matricula_siape = $autorizacao['nu_matricula_siape'];
            }
        } elseif($tipo == 'V'){
            
            $visitante = $this->recuperarVisitante($vttid, $vttdoc);

            $vttdoc = empty( $vttdoc )? $visitante['vttdoc']: $vttdoc;
            
            if( ctype_digit($vttdoc) )
            {
                    $visitanteIrrestrito                          = $this->recuperarNivelDas( $vttdoc );
                    if($visitanteIrrestrito){
                        
                        $servidor = $this->recuperarServidor( $vttdoc );
                        if( empty( $servidor[ 'ds_cargo_emprego' ] ) ) $servidor = $this->recuperarServidor( NULL, $vttdoc );
                        
                        //caso seja um SERVIDOR
                        $registroVisitante['vttid']                 = '';
                        $registroVisitante['nu_matricula_siape']    = $visitanteIrrestrito['nu_matricula_siape'];
                        $registroVisitante['vttdoc']                = empty( $visitanteIrrestrito['nu_cpf'] )? ''                   : $visitanteIrrestrito['nu_cpf'];
                        $registroVisitante['vttnome']               = empty( $visitanteIrrestrito['no_servidor' ] )? ''             : utf8_encode( $visitanteIrrestrito['no_servidor'] );
                        $registroVisitante['vttobs']                = '';
                        $registroVisitante['ds_cargo_emprego']      = empty( $servidor['ds_cargo_emprego'] )? ''                    : utf8_encode( $servidor['ds_cargo_emprego'] );
                        $registroVisitante['edaid']                 = '';
                        $registroVisitante['dstid']                 = '';
                        $registroVisitante['edfid']                 = '';
                        $registroVisitante['vstdatsaida']           = '';
                        $registroVisitante['andares']               = '';
                        $registroVisitante['vttnomeIrrestrito']     = empty( $visitanteIrrestrito['vttnomeIrrestrito' ] )?  ''      : utf8_encode( $visitanteIrrestrito['vttnomeIrrestrito'] );
                        $registroVisitante['vttfuncaoIrrestrito']   = empty( $visitanteIrrestrito['funcao' ] )? ''                  : utf8_encode( $visitanteIrrestrito['funcao'] );
                        $registroVisitante['vsnumcrachasistema']    = '';
                        $registroVisitante['status']                = 'ok';

                    }else{
                        
                        if($visitante){
                            $andares                                    = $this->recuperarAndaresEdificio($visitante['edfid']);
                            $ultimaVisita                               = $this->recuperaUltimaVisitaPeloIdVisitante( $visitante['vttid'] );
                            $registroVisitante['vttid']                 = $visitante['vttid'];
                            $registroVisitante['nu_matricula_siape']    = '';
                            $registroVisitante['vttdoc']                = empty( $visitante['vttdoc'] )? ''               : $visitante['vttdoc'];
                            $registroVisitante['vttnome']               = empty( $visitante['vttnome'] )? ''              : utf8_encode( $visitante['vttnome'] );
                            $registroVisitante['vttobs']                = empty( $visitante['vttobs'] )? ''               : utf8_encode( $visitante['vttobs'] );
                            $registroVisitante['ds_cargo_emprego']      = '';
                            $registroVisitante['edaid']                 = empty( $visitante['edaid'] )? ''                : $visitante['edaid']; 
                            $registroVisitante['dstid']                 = empty( $visitante['dstid'] )? ''                : $visitante['dstid']; 
                            $registroVisitante['edfid']                 = empty( $visitante['edfid'] )? ''                : $visitante['edfid']; 
                            $registroVisitante['vstdatsaida']           = empty( $visitante['vstdatsaida'] )? ''          : $visitante['vstdatsaida']; 
                            $registroVisitante['andares']               = empty( $andares )? ''                           : $andares; 
                            $registroVisitante['vttnomeIrrestrito']     = '';
                            $registroVisitante['vttfuncaoIrrestrito']   = '';
                            $registroVisitante['vsnumcrachasistema']    = empty( $ultimaVisita['vsnumcrachasistema'] )? ''   : $ultimaVisita['vsnumcrachasistema']; 
                            $registroVisitante['status']                = 'ok';
                        }else{
                            
                            $registroVisitante = array('status' => 'erro');
                            
                        }   

                   }
           
            }else{
                
                if($visitante){
                    //caso seja um VISITANTE
                    $andares                                    = $this->recuperarAndaresEdificio($visitante['edfid']);
                    $ultimaVisita                               = $this->recuperaUltimaVisitaPeloIdVisitante( $visitante['vttid'] );
                    $registroVisitante['vttid']                 = $visitante['vttid'];
                    $registroVisitante['nu_matricula_siape']    = '';
                    $registroVisitante['vttdoc']                = empty( $visitante['vttdoc'] )? ''                 : $visitante['vttdoc'];
                    $registroVisitante['vttnome']               = empty( $visitante['vttnome'] )? ''                : utf8_encode( $visitante['vttnome'] );
                    $registroVisitante['vttobs']                = empty( $visitante['vttobs'] )? ''                 : utf8_encode( $visitante['vttobs'] );
                    $registroVisitante['ds_cargo_emprego']      = '';
                    $registroVisitante['edaid']                 = empty( $visitante['edaid'] )? ''                  : $visitante['edaid']; 
                    $registroVisitante['dstid']                 = empty( $visitante['dstid'] )? ''                  : $visitante['dstid']; 
                    $registroVisitante['edfid']                 = empty( $visitante['edfid'] )? ''                  : $visitante['edfid']; 
                    $registroVisitante['vstdatsaida']           = empty( $visitante['vstdatsaida'] )? ''            : $visitante['vstdatsaida']; 
                    $registroVisitante['andares']               = empty( $andares )? ''                             : $andares; 
                    $registroVisitante['vttnomeIrrestrito']     = '';
                    $registroVisitante['vttfuncaoIrrestrito']   = '';
                    $registroVisitante['vsnumcrachasistema']    = empty( $ultimaVisita['vsnumcrachasistema'] )? ''  : $ultimaVisita['vsnumcrachasistema']; 
                    $registroVisitante['status']                = 'ok';
                    
                }else{
                    
                    $registroVisitante = array('status' => 'erro');
                    
                } 
                
            }
            
            $resultado = $registroVisitante;
            

        }else{
            $pessoa = $this->recuperarPessoa($vttid, $vttdoc, $nu_matricula_siape);

            if($pessoa){
                $resultado = array(
                    'vttid'   => $pessoa['vttid'],
                    'nu_matricula_siape'  => $pessoa['nu_matricula_siape'],
                    'vttdoc'  => $pessoa['vttdoc'],
                	'ds_cargo_emprego' => utf8_encode($pessoa['ds_cargo_emprego']),
                    'vttnome' => utf8_encode($pessoa['vttnome']),
                    'vttobs'  => utf8_encode($pessoa['vttobs']),
                    'status'  => 'ok');
            } else{
                $resultado = array('status' => 'erro');
            }
        }

        return $resultado;
    }

    public function listarUnidadePorFiltro($no_unidade_org){

        $where = "";
        if(!empty($no_unidade_org)){
            $where .= "and unidade.sg_resumida || ' - ' || unidade.no_unidade_org ilike '%$no_unidade_org%'";
        }

        $sql = "select '<center>
                        <a style=\"cursor:pointer;\" onclick=\"selecionarUnidade(' || unidade.co_interno_uorg || ', \'' || unidade.sg_resumida || ' - ' || unidade.no_unidade_org || '\');\">
                        <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>
                        </center>' as acao
                      ,unidade.sg_resumida
                      ,unidade.no_unidade_org
                  from siorg.tb_seo_unidade_org unidade
                 where ((unidade.tp_ligacao = 'S'
                   and unidade.status_org = true
                   and unidade.tp_ministerio = 1
                   and unidade.co_natureza_juridica in (1,3,4)
                   and unidade.co_interno_uorg <> 1
                   and unidade.sg_resumida not like 'CEFET%'
                   and unidade.no_unidade_org not like 'ESCOLA%'
                   and unidade.no_unidade_org not like 'FACULDADE%'
                   and unidade.no_unidade_org not like 'FUNDAção%'
                   and unidade.no_unidade_org not like 'UNIVERSIDADE%'
                   and unidade.no_unidade_org <> 'DIVIS?O'
                   and unidade.no_unidade_org <> 'DIVIS?O DE DOCUMENTAção'
                   and unidade.no_unidade_org <> 'SERVI?O')
                    or (unidade.co_interno_uorg in (389,1190,1015,391,1109,1216,1016,1222,1038,1027,1025)))
                $where
                 order by unidade.sg_resumida
                      ,unidade.no_unidade_org";

        $cabecalho = array("Ação", "Sigla", "Unidade Organizacional");
        $this->monta_lista($sql,$cabecalho,50,50,false,"center");
    }
    
    /**
     * Lista as Visitas que nao foram fechadas
     * @param unknown_type $vstdatentrada
     */
    public function listarVisitasPendentes( $vstdatentrada = false ){

        if($vstdatentrada){
            $vstdatentrada = ajusta_data($vstdatentrada);
            $where = " AND v.vstdatentrada > '{$vstdatentrada}' ";
        }
        
        $foto_url = 'http://'. $_SERVER['HTTP_HOST']. '/sca/sca.php?modulo=principal/visitante/mostrarFoto&acao=A'; 
        
        
        
        $sql = "
            SELECT
            	'<input type=\"checkbox\" name=\"visitas[]\" value=\"'||v.vstid||'\">' as acao,
            	(SELECT edadsc FROM sca.edificioandar WHERE edaid=v.edaid) || ' - '|| (SELECT dstsigla FROM sca.destino WHERE dstid=v.dstid) as localdestino, 
            	to_char(v.vstdatentrada,'dd/mm/YYYY HH24:MI'), 
            	( SUBSTR(v.vsnumcrachasistema, 5, 10)  ||' / '|| SUBSTR(v.vsnumcrachasistema, 0, 5) ) vsnumcrachasistema, 
            	vt.vttnome, td.tiddsc, vt.vttdoc, 
            	
            	
            	
                 CASE WHEN v.entrada=1 THEN 'EDIFICIO SEDE PRINCIPAL'
                   WHEN v.entrada=2 THEN 'EDIFICIO SEDE PRIVATIVA'
                   WHEN v.entrada=3 THEN 'ENTRADA ANEXO 1'
                    -- ELSE 'other'
                       END
                         as local_entrada
            	
            	--'<a href=\"javascript:visualizaFoto('|| v.vttid ||');\" >Foto</a>' as foto
            	/*
            	'
            	<div id=\"divfoto_'|| v.vstid ||'\"></div>
            	<script language=\"javascript\" type=\"text/javascript\">
            	$(\"#divfoto_'|| v.vstid ||'\").load(\"{$foto_url}&vttid='||v.vttid||'\");
            	</script>' as foto
            	*/
            	
            FROM 
            	sca.visita v
            LEFT JOIN sca.visitante vt ON vt.vttid=v.vttid
            LEFT JOIN sca.tipodocumento td ON vt.tidid = td.tidid 
            LEFT JOIN sca.edificioandar as edan ON v.edaid = edan.edaid
            WHERE 
            	vsnumcrachasistema IS NOT NULL
            	AND usucpfsaida IS NULL
            	AND vstdatsaida IS NULL
            	$where
			";
            	
        
                

        // Cabe?alho, local destino, data/hora, N?mero do Crachá, nome, foto     
        //$cabecalho = array("Ação", "Local Destino", "Data/Hora", "N?mero do Crachá", "Nome", "Foto");
        $cabecalho = array("Ação", "Local Destino", "Data/Hora", "Número do crachá", "Nome", "Tipo documento", "Documento","Local de Entrada");
        $this->monta_lista($sql,$cabecalho,50,50,false,"center", 'N', 'formularioVisitasPendentes');
    }

    /**
     * Limpra todas as visitas pendentes
     * @param unknown_type $visitas
     * @param unknown_type $usucpfsaida
     */
    public function efetuarSaidaVisitasPendentes( $visitas, $usucpfsaida ){
        if(sizeof($visitas)<1){
            return false;
        }
        
        $sql = '';
        foreach( $visitas as $Visita){
            //ver($Visita);
            $sql .="UPDATE sca.visita SET vstdatsaida='". date('Y-m-d H:i:s') ."', usucpfsaida='{$usucpfsaida}' WHERE vstid={$Visita};";
        }
        //ver($sql);
        try {
            $this->executar($sql);    
        } catch (Exception $e) {
            return false;
        }
        $this->commit();
        
        
    }
    

    public function montaComboAutorizados($autorizados){

       $sql = array();
       if($autorizados && count($autorizados) > 0){

            $vttid = array();
            $nu_matricula_siape = array();

            for($i = 0; $i < count($autorizados); $i++){
                if(!empty($autorizados[$i])){
                    $autorizado = explode('|', $autorizados[$i]);
                    if($autorizado[0] > 0){
                        $vttid[] = $autorizado[0];
                    } else{
                        $nu_matricula_siape[] = $autorizado[1];
                    }
                }
            }

            if(count($vttid) > 0 || count($nu_matricula_siape) > 0){
                $sql = "";
                if(count($vttid) > 0){
                    $sql = "select vttid || '|0' as codigo
                                  ,vttnome || ' (Visitante)' as descricao
                              from sca.visitante
                             where vttid in (" . implode(',', $vttid) . ")";
                }
                if(count($vttid) > 0 && count($nu_matricula_siape) > 0){
                    $sql .= " union ";
                }
                if(count($nu_matricula_siape) > 0){
                    $sql .= "select '0|' || nu_matricula_siape as codigo
                                   ,no_servidor || ' (Servidor)' as descricao
                               from sca.vwservidorativo
                              where nu_matricula_siape in (" . implode(',', $nu_matricula_siape) . ")";
                }
                $sql .= " order by descricao";
            }
        }
        
        $this->monta_combo_multiplo('autorizados',$sql,'S','','','','',10,400,array(),'autorizados[]');
    }

    public function salvarAutorizacao($autid, $co_uorg, $solicitante, $autdatinicio, $horainicio,
        $autdatfinal, $horafim, $auttelsolicitante, $autjustificativa, $autmemorando, $autorizados){

        $aux = explode('/',$autdatinicio);
        $autdatinicio = "$aux[2]-$aux[1]-$aux[0] $horainicio:00";
        $aux = explode('/',$autdatfinal);
        $autdatfinal = "$aux[2]-$aux[1]-$aux[0] $horafim:00";
        $auttelsolicitante = preg_replace('/[^0-9]/', '', $auttelsolicitante);
        $autmemorando = (!empty($autmemorando) ? (int) $autmemorando : 'null');

        if ($autid > 0){
            $sql = "update sca.autorizacao
                       set co_interno_uorg = $co_uorg
                          ,nu_matricula_siape = $solicitante
                          ,autdatinicio = '$autdatinicio'
                          ,autdatfinal = '$autdatfinal'
                          ,auttelsolicitante = '$auttelsolicitante'
                          ,autjustificativa = '$autjustificativa'
                          ,autmemorando = $autmemorando
                     where autid = $autid
                 returning autid";
        }
        else{
            $sql = "insert into sca.autorizacao(
                        co_interno_uorg,nu_matricula_siape,autdatinicio,autdatfinal,auttelsolicitante,autjustificativa,autmemorando)
                    values($co_uorg,$solicitante,'$autdatinicio','$autdatfinal','$auttelsolicitante','$autjustificativa',$autmemorando)
                    returning autid";
        }

        $autid = $this->pegaUm($sql);

        $sql = "delete from sca.autorizado where autid = $autid";

        $this->executar($sql);

        

        foreach($autorizados as $autorizado){
            
            $id = explode('|',$autorizado[0]);
            $vttid = ($id[0] > 0 ? $id[0] : 'null');
            $nu_matricula_siape = ($id[1] > 0 ? $id[1] : 'null');
            $sql = "insert into sca.autorizado(autid, vttid, nu_matricula_siape)
                    values($autid,$vttid,$nu_matricula_siape)";

            $this->executar($sql);

        }

        $this->commit();
        direcionar('?modulo=principal/visitante/autorizarAcessoForaHorario&acao=A','Operação realizada com sucesso!');
    }

    public function carregaAutorizacao($autid){

        $sql = "select t1.autid, t1.co_interno_uorg, t1.nu_matricula_siape
                      ,to_char(t1.autdatinicio,'DD/MM/YYYY') as autdatinicio
                      ,to_char(t1.autdatinicio,'HH24:MI') as horainicio
                      ,to_char(t1.autdatfinal,'DD/MM/YYYY') as autdatfinal
                      ,to_char(t1.autdatfinal,'HH24:MI') as horafim
                      ,t1.auttelsolicitante, t1.autjustificativa, t1.autmemorando
                      ,t2.no_servidor as autnomesolicitante, t2.no_unidade_org
                  from sca.autorizacao t1
                 inner join sca.vwservidorativo t2
                    on t1.nu_matricula_siape = t2.nu_matricula_siape
                 where t1.autid = $autid";

        $autorizacao = $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );

        $sql = "select t1.vttid, t1.nu_matricula_siape
                  from sca.autorizado t1
                 where t1.autid = $autid";

        $autorizados = $this->carregar(iconv("UTF-8", "ISO-8859-1", $sql));
        $autorizacao['autorizados'] = array();

        for($i = 0; $i < count($autorizados); $i++){
            $vttid = (empty($autorizados[$i]['vttid']) ? '0' : $autorizados[$i]['vttid']);
            $nu_matricula_siape = (empty($autorizados[$i]['nu_matricula_siape']) ? '0' : $autorizados[$i]['nu_matricula_siape']);

            $autorizacao['autorizados'][] = $vttid . "|" . $nu_matricula_siape;
        }

        return $autorizacao;
    }

    public function carregaAutorizado($audid, $nu_matricula_siape){

        if(!empty($nu_matricula_siape))
        {
            $sql = "select t2.audid
                      from sca.autorizacao t1
                     inner join sca.autorizado t2
                        on t1.autid = t2.autid
                     where t2.nu_matricula_siape = $nu_matricula_siape
                       and now() between t1.autdatinicio and t1.autdatfinal
                     limit 1";

            $audid = $this->pegaUm($sql);

            if(empty($audid)){

                $sql = "insert into sca.autorizacao(co_interno_uorg, nu_matricula_siape, autdatinicio, autdatfinal, autjustificativa)
                            select co_interno_uorg, nu_matricula_siape, now()::date
                                  ,to_timestamp(to_char(now(),'DD/MM/YYYY') || ' 23:59', 'DD/MM/YYYY HH24:MI')
                                  ,'AutorizAção permanente. ' || coalesce(ds_cargo_emprego,'')
                              from sca.vwservidorativo
                             where nu_matricula_siape = $nu_matricula_siape
                         returning autid ";

                $autid = $this->pegaUm($sql);

                $sql = "insert into sca.autorizado(autid, nu_matricula_siape)
                        values($autid, $nu_matricula_siape)
                        returning audid";
                $audid = $this->pegaUm($sql);

                $this->commit();
            }
        }

        $sql = "select autorizado.audid
                      ,autorizado.vttid
                      ,autorizado.nu_matricula_siape
                      ,coalesce(visitante.vttdoc, servidor.nu_cpf) as vttdoc
                      ,coalesce(visitante.vttnome, servidor.no_servidor) as vttnome
                      ,coalesce(visitante.vttobs, servidor.ds_cargo_emprego) as vttobs
                      ,visita.vstid, visita.edaid, visita.dstid, andar.edfid
                  from sca.autorizado autorizado
                  left join sca.visitante visitante
                    on autorizado.vttid = visitante.vttid
                  left join sca.visita visita
                    on visitante.vttid = visita.vttid
                  left join sca.edificioandar andar
                    on visita.edaid = andar.edaid
                  left join sca.vwservidorativo servidor
                    on autorizado.nu_matricula_siape = servidor.nu_matricula_siape
                 where autorizado.audid = $audid
                 order by visita.vstid desc
                 limit 1";

        return $this->pegaLinha(iconv("UTF-8", "ISO-8859-1", $sql));
    }

    public function consultarSaidaPorDocumentoCracha($vttid, $vttdoc, $vstid){

    	if (!empty($vstid)){
    		$where[] = " visita.vstid = $vstid ";
    	}
    	
    	if(!empty($vttid)){
    		$where[] = " visitante.vttid = $vttid ";
    	}
    	
        if(!empty($vttdoc)){
        	
        	 $novo_numero = date('Y') . (str_pad($vttdoc , 6, "0", STR_PAD_LEFT)) ;
        	 
                if(ctype_digit($vttdoc)){
                    $where[] = " (visita.vstnumcracha = $vttdoc OR visita.vsnumcrachasistema = '$novo_numero' OR visitante.vttdoc = '$vttdoc') ";
                } else{
                    $where[] = " (visita.vsnumcrachasistema = '$novo_numero' OR visitante.vttdoc = '$vttdoc') ";
                }
    	}
    	
        $sql = "select visita.vstid
                      ,visitante.vttid
                      ,visitante.vttdoc
                      ,visitante.vttnome
                      ,visitante.vttobs
                  from sca.visita visita
                 inner join sca.visitante visitante
                    on visitante.vttid = visita.vttid
                 where visita.vstdatsaida is null and (" . implode("OR", $where) . ")";
        
        return $this->carregar($sql);
    }

    public function consultarSaidaAutorizados($vttid, $vttdoc, $vstid, $nu_matricula_siape){

        if ($vstid && !empty($vstid)){

            $sql = "select visita.vstid
                          ,visitante.vttid
                          ,null as nu_matricula_siape
                          ,coalesce(visitante.vttdoc, servidor.nu_cpf) as vttdoc
                          ,coalesce(visitante.vttnome, servidor.no_servidor) as vttnome
                          ,coalesce(visitante.vttobs, servidor.ds_cargo_emprego) as vttobs
                      from sca.visita visita
                      left join sca.visitante visitante
                        on visitante.vttid = visita.vttid
                      left join sca.autorizado autorizado
                     inner join sca.vwservidorativo servidor
                        on autorizado.nu_matricula_siape = servidor.nu_matricula_siape
                        on visita.audid = autorizado.audid
                     where visita.vstid = $vstid
                     limit 1";

            return $this->carregar($sql);

        } elseif ($vttid && !empty($vttid)){

            $sql = "select coalesce(visita.vstid, -1) as vstid
                          ,visitante.vttid
                          ,null as nu_matricula_siape
                          ,visitante.vttdoc
                          ,visitante.vttnome
                          ,visitante.vttobs
                      from sca.visitante visitante
                      left join sca.visita visita
                        on visitante.vttid = visita.vttid
                       and visita.vstdatsaida is null
                     where visitante.vttid = $vttid
                     order by visita.vstid desc
                     limit 1";

            return $this->carregar($sql);

        } elseif ($nu_matricula_siape && !empty($nu_matricula_siape)){

            $sql = "select coalesce(visita.vstid, -1) as vstid
                          ,null as vttid
                          ,servidor.nu_matricula_siape
                          ,servidor.nu_cpf as vttdoc
                          ,servidor.no_servidor as vttnome
                          ,servidor.ds_cargo_emprego as vttobs
                      from sca.vwservidorativo servidor
                      left join sca.autorizado autorizado
                        on servidor.nu_matricula_siape = autorizado.nu_matricula_siape
                      left join sca.autorizacao autorizacao
                        on autorizado.autid = autorizacao.autid
                       and now() between autorizacao.autdatinicio and autorizacao.autdatfinal
                      left join sca.visita visita
                        on autorizado.audid = visita.audid
                       and visita.vstdatsaida is null
                     where servidor.nu_matricula_siape = $nu_matricula_siape
                     order by visita.vstid desc
                     limit 1";

            return $this->carregar($sql);

        } else{

            $vstnumcracha = $vttdoc == (string)((int) $vttdoc) ? "or visita.vstnumcracha = $vttdoc" : "";
            $sql = "select * from (select coalesce(visita.vstid, -1) as vstid
                                         ,null::integer as vttid
                                         ,servidor.nu_matricula_siape
                                         ,servidor.nu_cpf as vttdoc
                                         ,servidor.no_servidor as vttnome
                                         ,servidor.ds_cargo_emprego as vttobs
                                     from sca.vwservidorativo servidor
                                     left join sca.visita visita
                                       on servidor.nu_matricula_siape = visita.nu_matricula_siape
                                      and visita.vstdatsaida is null
                                    where servidor.nu_cpf = '$vttdoc') as t1
                    union
                    select * from (select coalesce(visita.vstid, -1) as vstid
                                         ,visitante.vttid
                                         ,null::integer as nu_matricula_siape
                                         ,visitante.vttdoc
                                         ,visitante.vttnome
                                         ,visitante.vttobs
                                     from sca.visitante visitante
                                     left join sca.visita visita
                                       on visitante.vttid = visita.vttid
                                      and visita.vstdatsaida is null
                                    where visitante.vttdoc = '$vttdoc' $vstnumcracha) as t2";
          

            return $this->carregar($sql);
        }
    }

    /**
     * M?todo respons?vel por montar a combo do andares do edif?cio
     *
     * @name montaComboEdificio
     * @author C?zar Cirqueira
     * @access public
     * @return select com todos andares do edif?cio
     */
    public function montaComboAndarEdificio($edfid,$edaid){

        $resultado = '';

        if(!empty($edfid) && $edfid != ''){

            $sql = "SELECT edaid as codigo, edadsc as descricao
                      FROM sca.edificioandar an
                      JOIN sca.edificio ed
                          ON ed.edfid = an.edfid
                      WHERE
                          an.edfid = $edfid
                      ORDER BY 2";

            $resultado = $this->carregar($sql);
        }

        //caso n?o encontre registro, passa um array vazio para o componente
        if(!is_array($resultado)){
            $resultado = array();
        }

        $this->monta_combo("edaid",$resultado,'S',"Selecione...","","","","303","S","edaid","",$edaid);
    }



    /**
     * M?todo respons?vel por montar a combo do destino do visitante
     *
     * @name montaComboDestino
     * @author C?zar Cirqueira
     * @access public
     * @return select com todos destinos
     */
    public function montaComboDestino(){
        $sql = "select dstid as codigo
                      ,dstsigla || ' - ' || dstdsc as descricao
                  from sca.destino
                 order by 2";

        $this->monta_combo("dstid",$sql,'S',"Selecione...","","","","303","S","dstid","",$dstid);
    }

    /**
     * M?todo respons?vel por montar a combo do equipamento
     *
     * @name montaComboEquipamento
     * @author Alysson Rafael
     * @access public
     * @return select multiple com Equipamentos do Visitante
     */
    public function montaComboEquipamento($eqmid){

        if($eqmid && count($eqmid) > 0){

            $equipamentos = array();

            for($i = 0; $i < count($eqmid); $i++){
                if(is_numeric($eqmid[$i][0])){
                    $equipamentos[] = $eqmid[$i][0];
                }
            }
            if(count($equipamentos) > 0){
                $sql = "select t1.eqmid as codigo
                              ,t2.tpedsc || ' - ' || t3.mcedsc || ' - ' || t1.eqmnumserie as descricao
                          from sca.equipamento t1
                         inner join sca.tipoequipamento t2
                            on t1.tpeid = t2.tpeid
                         inner join sca.marcaequipamento t3
                            on t1.mceid = t3.mceid
                         where t1.eqmid in (" . implode(',', $equipamentos) . ")";

                $items = $this->carregar($sql);
            }
        }

        combo_popup( 'eqmid[]', $sql, 'Selecione o(s) Equipamento(s)', '360x460', 0,array(), '', 'S', false, false, 10, 350, null, null, false, null, $items, true,false, null, false, null, null);
    }

    /**
     * M?todo respons?vel por buscar o visitante por id
     *
     * @name carregaVisitantePorId
     * @author C?zar Cirqueira
     * @access public
     * @return visitante
     */
    public function carregaVisitantePorId($vttid){
        $mensagem = "";
        $sql = "SELECT distinct vttid, tidid, ogeid, arqid, vttnome, vttobs, vtttel, vttdoc, usucpf, vttdatcadastro
                  FROM sca.visitante
                  WHERE vttid = '$vttid';
        ";
        return $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
    }

    public function validarHorarioEntrada( $_POSTDADOS )
    {
        # Valor inicial para mensgaem de erro
        $strMensagemValidacao = NULL;
        
        # Busca dados caso o visitante seja do tipo 'Irrestrito'
        $visitanteIrrestrito = $this->recuperarNivelDas( $_POSTDADOS[ 'vttdoc' ] );
        if( !empty( $visitanteIrrestrito[ 'no_servidor' ] ) ) return $strMensagemValidacao;
        
        # Verifica se o visitante estah entrando em horario fora do expediente
        $sql = "
            SELECT
                (
                CASE WHEN(
                    -- Verifica horario fora do expediente
                    ( EXTRACT( HOUR FROM NOW() ) < 6 OR EXTRACT( HOUR FROM NOW() ) > 20 )
                    OR
                    -- Verifica finais de semana
                    ( TO_CHAR( now(), 'D' )::INT = 1 OR TO_CHAR( now(), 'D' )::INT = 7 )
                )THEN
                    1
                ELSE
                    0
                END
                ) AS nu_fora_expediente,
                TO_CHAR( NOW(), 'DD/MM/YYYY' ) AS dt_entrada
        ";
        $arrHorario = $this->pegaLinha( $sql );

        # Verifica se a visita estah acontecendo em horario fora do expediente
        if( !empty( $arrHorario[ 'nu_fora_expediente' ] ) )
        {
            $objVisitante = new Visitante();
            # Verifica se o visitante possui altorizacao para trabalhar fora do horario de expediente
            $arrAutorizacao = $objVisitante->listarAutorizacoesPorFiltro(
                $_POSTDADOS[ 'vttdoc' ],
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                $arrHorario[ 'dt_entrada' ],
                TRUE
            );

            if( $arrAutorizacao === FALSE )
            {
                $strMensagemValidacao = 'O visitante não tem autorização para entrar após expediente';
            }
        }

        return $strMensagemValidacao;
    }
    
    public function salvarEntradaVisitante($_POSTDADOS, $usucpf, $imagemVisitante, $redireciona = true){

        $mensagemValidacao = "";
        $equipamentos = array();

        for($i = 0; $i < count($_POSTDADOS['eqmid']); $i++){
            $id = $_POSTDADOS['eqmid'][$i][0];
            if(!empty($id)){
                $equipamentos[] = $id;
            }
        }
        
        # Verifica se o visitante estah entrando em horario fora do expediente
        $mensagemValidacao = $this->validarHorarioEntrada( $_POSTDADOS );
        
        if( empty( $mensagemValidacao ) )
        {
            $mensagemValidacao = $this->validarEntrada($_POSTDADOS['vttid'] ,$_POSTDADOS['audid'],
                $_POSTDADOS['vstnumcracha'] ,$_POSTDADOS['edaid'], $equipamentos);
        }

        if(empty($mensagemValidacao)){
            extract($_POST);
            $sql = "";

            $vstid = (!empty($vstid))?$vstid:'null';
            $vttid = (!empty($vttid))?$vttid:'';
            $nu_matricula_siape = (!empty($nu_matricula_siape))?$nu_matricula_siape:'';
            $audid = (!empty($audid))?$audid:'';
            $edaid = (!empty($edaid))?$edaid:'null';
            $dstid = (!empty($dstid))?$dstid:'null';
    
            // Numero Etiqueta
            if( empty($vstnumcracha) || $vstnumcracha==''){
                $vsnumcrachasistema = $this->geraNumeroCrachaSistema();    
            }else{
                $vsnumcrachasistema = '';
            }

            $vstnumcracha = (!empty($vstnumcracha))?$vstnumcracha:'null';
            
            $usucpf = (!empty($usucpf))?$usucpf:'';

            if(empty($entrada)){
                $entrada = "NULL";
            }

            if(!empty($audid)){
                $servidor = $nu_matricula_siape;
                $visitante = $vttid;

                if(!empty($vttid)){
                    $servidor = 'null';
                    $insertFoto = array("vttid" => $vttid);
                }
                else{
                    $visitante = 'null';
                    $insertFoto = array("nu_matricula_siape" => $nu_matricula_siape);
                }

                $sql = "insert into sca.visita(vttid,nu_matricula_siape,edaid,dstid,audid,vstnumcracha,usucpfentrada,vstdatentrada, entrada, vsnumcrachasistema)
                        values ($visitante,$servidor,$edaid,$dstid,$audid,$vstnumcracha,'$usucpf','". date('Y-m-d H:i:s') ."', $entrada, '$vsnumcrachasistema')";
                
                $this->executar($sql);
            }
            else
            {
                # Verifica se o visitante jah eh cadastrado
                if( empty( $vttid ) )
                {
                    # Insere o visitante
                    $sql = "
                    INSERT INTO sca.visitante(
                        tidid,
                        ogeid,
                        vttnome,
                        vttobs,
                        vtttel,
                        vttdoc,
                        usucpf
                        $insertNameUorg
                        $insertNameTerc
                    )
                    VALUES (
                        " . ( empty( $tidid )? 'NULL': $tidid ) . ",
                        " . ( empty( $ogeid )? 'NULL': $ogeid ) . ",
                        '$vttnome',
                        " . ( empty( $vttobs )? 'NULL': "'$vttobs'" ) . ",
                        " . ( empty( $vtttel )? 'NULL': "'$vtttel'" ) . ",
                        " . ( empty( $vttdoc )? 'NULL': "'$vttdoc'" ) . ",
                        " . ( empty( $usucpf )? 'NULL': "'$usucpf'" ) . "
                        $insertValueUorg
                        $insertValueTerc )
                    ";
                    $sql .=" returning vttid";
                    $vttid = $this->pegaUm( $sql );
                }

                # Insere o id do visitante para caso exista imagem a ser salva
                $insertFoto = array("vttid" => $vttid);

                $sql = "insert into sca.visita(vttid,edaid,dstid,vstnumcracha,usucpfentrada,vstdatentrada, entrada,vsnumcrachasistema)
                        values ($vttid,$edaid,$dstid,$vstnumcracha,'$usucpf','". date('Y-m-d H:i:s') ."', $entrada, '$vsnumcrachasistema')";

                $this->executar($sql);
            }

            # Verifica se existe imagem para salvar ao registrar visita
            if(!empty($imagemVisitante)){
                $foto = new FilesSimec("visitantefoto", $insertFoto, "sca");
                $foto->setStream("Foto de visitante", $imagemVisitante);
            }

            # Verifica se existe equipamentos para cadastrar ao registrar visita
            if( count( $equipamentos ) > 0 )
            {
                $oEquipamento = new Equipamento();
                $oEquipamento->salvarEntradaEquipamento($vttid, $nu_matricula_siape, $equipamentos, $usucpf);
            }
            else
            {
                $this->commit();
                $redireciona = true;
            }
        }
        else
        {
            return $mensagemValidacao;
        }

        $this->recuperaUltimaVisitaInserida();

        if( $redireciona == true )
        {
            return "Operação realizada com sucesso!";
            //direcionar('?modulo=principal/visitante/registrarEntradaVisitante&acao=A','');
        }

    }

    public function salvarSaidaVisitante($_POSTDADOS, $usucpf, $imagemVisitante){

        extract($_POSTDADOS);

        $vstid = (!empty($vstid))?$vstid:'-1';
        $vttid = (!empty($vttid))?$vttid:'null';
        $nu_matricula_siape = (!empty($nu_matricula_siape))?$nu_matricula_siape:'null';

        $edaid = (!empty($edaid))?$edaid:'';
        $dstid = (!empty($dstid))?$dstid:'';
        $numsalforahorario = (!empty($numsalforahorario))?$numsalforahorario:'';
        $usucpf = (!empty($usucpf))?$usucpf:'';

        if(!empty($edaid) && !empty($dstid) && !empty($numsalforahorario)){
            if(!empty($vstid) && $vstid > 0){

                $sql = "update sca.visita
                           set vstdatsaida = '". date('Y-m-d H:i:s') ."'
                              ,usucpfsaida = '$usucpf'
                              ,edaidforahorario = $edaid
                              ,dstidforahorario = $dstid
                              ,numsalforahorario = $numsalforahorario
                         where vstid = $vstid";

            } else {
                $sql = "insert into sca.visita(vttid,nu_matricula_siape,vstdatsaida,usucpfsaida,edaidforahorario,dstidforahorario,numsalforahorario)
                        values ($vttid,$nu_matricula_siape,'". date('Y-m-d H:i:s') ."','$usucpf',$edaid,$dstid,'$numsalforahorario')";
            }

            $this->executar($sql);
        } else {

            $sql = "update sca.visita
                       set vstdatsaida = '". date('Y-m-d H:i:s') ."'
                          ,usucpfsaida = '$usucpf'
                     where vstid = $vstid";

            $this->executar($sql);
        }

        if(!empty($imagemVisitante)){
            if($vttid != 'null'){
                $insertFoto = array("vttid" => $vttid);
            }else if($nu_matricula_siape != 'null'){
                $insertFoto = array("nu_matricula_siape" => $nu_matricula_siape);
            }

            $foto = new FilesSimec("visitantefoto", $insertFoto, "sca");
            $foto->setStream("Foto de visitante", $imagemVisitante);
        }

        if(!empty($_POSTDADOS['eqmid'])){
            $oEquipamento = new Equipamento();
            $oEquipamento->salvarSaidaEquipamento($vttid, $nu_matricula_siape, $_POSTDADOS['eqmid'], $usucpf);
        }else{
            $this->commit();
        }

        direcionar('?modulo=principal/visitante/registrarSaidaVisitante&acao=A','Operação realizada com sucesso!');
    }


    public function validarEntrada($vttid, $audid, $vstnumcracha, $edaid, $equipamentos){
        $mensagemValidacao = "";
        $oEquipamento = new Equipamento();

        //Valida Crachá
        if(!empty($vstnumcracha)){

            $sql = "SELECT vstnumcracha
                      FROM sca.visita
                     WHERE vstnumcracha = $vstnumcracha
                       AND edaid = $edaid
                       AND vstdatsaida is NULL";

            $crachaValido = $this->eof($sql);
            $mensagemValidacao = ($crachaValido)?"":"O número do Crachá informado está associado a outro visitante neste edifício.";
        }

        //Valida visitante
        if(empty($mensagemValidacao)){
            if(!empty($vttid)){
                $sql = "SELECT vstid
                          FROM sca.visita
                         WHERE vttid = $vttid
                           AND vstdatsaida is NULL";

                $visitanteValido = $this->eof($sql);
                $mensagemValidacao = ($visitanteValido)?"":"Não existe registro de saída para a Última visita do visitante informado.";
            }
            else if(!empty($audid)){

                //autorizados
                $sql = "SELECT vstid
                          FROM sca.visita
                         WHERE audid = $audid
                           AND vstdatsaida is NULL";

                $autorizadoValido = $this->eof($sql);
                $mensagemValidacao = ($autorizadoValido)?"":"Não existe registro de saída para a Última visita do visitante informado.";
            }
        }

        //valida equipamento
        if(empty($mensagemValidacao)){
            //verifica os equipamentos com registro de entrada sem sa?da
            for($i = 0; $i < count($equipamentos); $i++){
                if(!empty($equipamentos[$i])){
                    if($oEquipamento->verificaEntradaSemSaida($equipamentos[$i])){
                        $mensagemValidacao = "Não existe registro de saída para o equipamento informado.";
                        break;
                    }
                }
            }
        }

        return $mensagemValidacao;
    }

    /**
     * Metodo responsavel por buscar os dados do Último registro de entrada de um visitante
     *
     * @name buscarUltimoAcesso
     * @param $vttid  - id do visitante
     * @param $vttdoc - documento do visitante
     * @author C?zar
     * @return array com os dados recuperados
     */
    function buscarUltimoAcesso($vttid, $vttdoc){

        $visitaAnterior = null;
        $condicao = "";
        if($vttid!=null && !empty($vttid)){
             $condicao .=" WHERE 1=1 AND visitante.vttid = $vttid";
        }elseif($vttdoc!=null && !empty($vttdoc)){
             $condicao .=" WHERE 1=1 AND visitante.vttdoc = '$vttdoc'";
        }
        if(!empty($condicao)){
            $sql  = "SELECT visita.vttid,visita.edaid,visita.dstid,visita.vstdatentrada,edificioandar.edfid ";
            $sql .= "FROM sca.visita visita ";
            $sql .= "JOIN sca.visitante visitante ON (visitante.vttid = visita.vttid) ";
            $sql .= "JOIN sca.edificioandar edificioandar ON (edificioandar.edaid = visita.edaid) ";
            $sql .= $condicao." ORDER BY visita.vstid DESC LIMIT 1";
            $visitaAnterior = $this->carregar($sql);
        }
        return ($visitaAnterior!=null && !empty($visitaAnterior))? $visitaAnterior : array();
    }

    public function montaListaSaidaPorDocumentoCracha($vttdoc){

        $vstnumcracha = $vttdoc == (string)((int) $vttdoc) ? "or visita.vstnumcracha = $vttdoc" : "";

        //$cracha = new CrachaProvisorio();
        //$expediente = $cracha->verificarExpedienteTrabalho();
        $expediente = SCA_EXPEDIENTE_NORMAL;

        if($expediente == SCA_EXPEDIENTE_NORMAL){
            $where = "where visita.vstdatsaida is null and (";
            $where .= "visitante.vttdoc = '$vttdoc' $vstnumcracha)";

            $sql = "select '<center>
                                <a style=\"cursor:pointer;\" onclick=\"selecionarPessoa('||visitante.vttid||',null);\">
                                <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>
                            </center>' as acao
                          ,visita.vstnumcracha
                          ,case when td.tiddsc = 'CPF' and length(visitante.vttdoc) = 11
                                then substr(visitante.vttdoc,1,3) || '.' || substr(visitante.vttdoc,4,3) || '.' || substr(visitante.vttdoc,7,3) || '-' || substr(visitante.vttdoc,10,2)
                                else visitante.vttdoc
                           end vttdoc
                          ,visitante.vttnome
                          ,e.edfdsc || ' - ' || ea.edadsc as destino
                      from sca.visita visita
                     inner join sca.visitante visitante
                        on visitante.vttid = visita.vttid
                     inner join sca.edificioandar ea
                        on visita.edaid = ea.edaid
                     inner join sca.edificio e
                        on ea.edfid = e.edfid
                      left join sca.tipodocumento td
                        on td.tidid = visitante.tidid
                    $where";

            $registros = $this->carregar($sql);

            $cabecalho = array("Ação", "Crachá", "Documento", "Nome", "Destino");
            $this->monta_lista_simples($registros, $cabecalho, 100, 50);

        } else {
            $sql = "select * from (select '<center>
                                                <a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(null,'||servidor.nu_matricula_siape||');\">
                                                <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>
                                            </center>' as acao
                                         ,visita.vstnumcracha
                                         ,servidor.nu_matricula_siape
                                         ,substr(servidor.nu_cpf,1,3) || '.' || substr(servidor.nu_cpf,4,3) || '.' || substr(servidor.nu_cpf,7,3) || '-' || substr(servidor.nu_cpf,10,2) as vttdoc
                                         ,servidor.no_servidor as vttnome
                                         ,e.edfdsc || ' - ' || ea.edadsc as destino
                                     from sca.vwservidorativo servidor
                                     left join sca.visita visita
                                       on servidor.nu_matricula_siape = visita.nu_matricula_siape
                                      and visita.vstdatsaida is null
                                     left join sca.edificioandar ea
                                       on visita.edaid = ea.edaid
                                     left join sca.edificio e
                                       on ea.edfid = e.edfid
                                    where servidor.nu_cpf = '$vttdoc' $vstnumcracha) as t1
                    union
                    select * from (select '<center>
                                                <a style=\"cursor:pointer;\" onclick=\"selecionarPessoa('||visitante.vttid||',null);\">
                                                <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>
                                            </center>' as acao
                                         ,visita.vstnumcracha
                                         ,null::integer as nu_matricula_siape
                                         ,case when td.tiddsc = 'CPF' and length(visitante.vttdoc) = 11
                                               then substr(visitante.vttdoc,1,3) || '.' || substr(visitante.vttdoc,4,3) || '.' || substr(visitante.vttdoc,7,3) || '-' || substr(visitante.vttdoc,10,2)
                                               else visitante.vttdoc
                                          end vttdoc
                                         ,visitante.vttnome
                                         ,e.edfdsc || ' - ' || ea.edadsc as destino
                                     from sca.visitante visitante
                                     left join sca.visita visita
                                       on visitante.vttid = visita.vttid
                                      and visita.vstdatsaida is null
                                     left join sca.edificioandar ea
                                       on visita.edaid = ea.edaid
                                     left join sca.edificio e
                                       on ea.edfid = e.edfid
                                     left join sca.tipodocumento td
                                       on td.tidid = visitante.tidid
                                    where visitante.vttdoc = '$vttdoc' $vstnumcracha) as t2";

            $registros = $this->carregar($sql);

            $cabecalho = array("Ação", "Crachá", "Matrícula SIAPE", "Documento", "Nome", "Destino");
            $this->monta_lista_simples($registros, $cabecalho, 100, 50);

        }
    }

    public function consultarSaidaEquipamento($vttid, $nu_matricula_siape = null, $eqmnumetiqueta = null){

        $where = "where movimentacao.mvedatsaida is null and ";

        if ($vttid && !empty($vttid)){
            $where .= "movimentacao.vttid = $vttid";
        } elseif ($nu_matricula_siape && !empty($nu_matricula_siape)){
            $where .= "movimentacao.nu_matricula_siape = $nu_matricula_siape";
        } elseif ($eqmnumetiqueta && !empty($eqmnumetiqueta)){
            $where .= "exists(select t1.mveid
                                from sca.movimentacaoequipamento t1
                                join sca.equipamento t2
                                  on t1.eqmid = t2.eqmid
                               where t1.mvedatsaida is null
                                 and t2.eqmnumetiqueta = '$eqmnumetiqueta'
                                 and (t1.vttid = movimentacao.vttid
                                  or t1.nu_matricula_siape = movimentacao.nu_matricula_siape))";
        } else{
            $where .= "1 = 2";
        }

        $sql = "select movimentacao.mveid
                      ,tipo.tpedsc
                      ,marca.mcedsc
                      ,equipamento.eqmnumserie
                      ,equipamento.eqmnumetiqueta
                      ,movimentacao.vttid
                      ,movimentacao.nu_matricula_siape
                  from sca.movimentacaoequipamento movimentacao
                  join sca.equipamento equipamento
                    on equipamento.eqmid = movimentacao.eqmid
                  join sca.marcaequipamento marca
                    on equipamento.mceid = marca.mceid
                  join sca.tipoequipamento tipo
                    on equipamento.tpeid = tipo.tpeid
                $where
                 order by movimentacao.mveid";

        return $this->carregar($sql);
    }

    public function pegaListaVisitantes($co_interno_uorg){
        
        $sql = "SELECT
                    vttid || '|0' as codigo,
                    vttnome || ' (Terceirizado) ' as descricao
                FROM
                    sca.visitante vis
                WHERE
                    vttterceiro=true
                AND
                    co_uorg=$co_interno_uorg

                UNION

                SELECT
                    '0|' || nu_matricula_siape as codigo,
                    no_servidor || ' (Servidor) ' as descricao
                FROM
                    sca.vwservidorativo
                WHERE
                    co_interno_uorg='$co_interno_uorg'";

        
        return $this->carregar($sql);

    }

    public function pegarListaVisitantesSelecionados($autorizados, $co_interno_uorg){

        $in = '0';

        if(count($autorizados) > 0){

            foreach($autorizados as $linha=>$autorizado){

                $in .= ',' . $autorizado[0];
                
            }
            
        }

        $sql = "SELECT
                    vttid || '|0' as codigo,
                    vttnome || ' (Terceirizado) ' as descricao
                FROM
                    sca.visitante vis
                WHERE
                    vttterceiro=true
                AND
                    co_uorg=$co_interno_uorg
                AND
                    vttid in ($in)

                UNION

                SELECT
                    '0|' || nu_matricula_siape as codigo,
                    no_servidor || ' (Servidor) ' as descricao
                FROM
                    sca.vwservidorativo
                WHERE
                    co_interno_uorg = $co_interno_uorg
                AND
                    nu_matricula_siape in ($in)";


        return $this->carregar($sql);

    }
    
    public function recuperarNivelDas ( $numeroDocumento ) {
    	
        $nu_matricula_siape = $numeroDocumento;
        
    	$sql = "SELECT no_servidor,nu_cpf, nu_matricula_siape, co_funcao
    					, co_nivel_funcao, no_unidade_org,
		    	CASE
			    	WHEN co_nivel_funcao = 1014 THEN 'DAS NÍVEL 4'
			    	WHEN co_nivel_funcao = 1024 THEN 'DAS NÍVEL 4'
			    	WHEN co_nivel_funcao = 1015 THEN 'DAS NÍVEL 5'
			    	WHEN co_nivel_funcao = 1025 THEN 'DAS NÍVEL 5'
			    	WHEN co_nivel_funcao = 1015 THEN 'DAS NÍVEL 5'
			    	WHEN co_nivel_funcao = 1016 THEN 'DAS NÍVEL 6'
		    	END as funcao
	    	FROM sca.vwservidorativo servidor
	    	WHERE
		    	( servidor.ds_cargo_emprego ilike 'MINISTRO DE ESTADO%'
		    	or (servidor.co_funcao = 'DAS'
		    	and servidor.co_nivel_funcao in (1014,1015,1016,1024,1025,1026)
		    	)
	    	OR (servidor.co_funcao = 'NES')
	    	OR (servidor.ds_cargo_emprego ilike 'CHEFE DE GABINETE%'))";
                
            if(ctype_digit($nu_matricula_siape)){
                $sql .= "AND (nu_cpf = '{$numeroDocumento}' OR nu_matricula_siape = '{$nu_matricula_siape}' )";
            }else{
                $sql .= "AND (nu_cpf = '{$numeroDocumento}' )";
            }
        
                
                
    	return $this->pegaLinha($sql);
    }
  
}
