<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Equipamento extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sca.equipamento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("eqmid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                       'eqmid' => null,
                                       'tpeid' => null,
                                       'mceid' => null,
                                       'eqmnumserie' => null,
                                       'eqmnumetiqueta' => null,
                                       'usucpf' => null,
                                       'eqmdatcadastro'=>null
    		                           
                                      );

    public function consultarEquipamento($eqmnumetiqueta){

    	

        $sql = "select e.eqmid as codigo
                      ,t.tpedsc || ' - ' || m.mcedsc || ' - ' || e.eqmnumserie as descricao
                  from sca.equipamento e
                  join sca.marcaequipamento m
                    on e.mceid = m.mceid
                  join sca.tipoequipamento t
                    on e.tpeid = t.tpeid
                 where e.eqmnumetiqueta = '" . strtoupper($eqmnumetiqueta) . "'";

        return $this->pegaLinha($sql);
    }

    /**
     * Método responsável por montar a combo de tipo de equipamento
     *
     * @name montaComboTipoEquipamento
     * @author Alysson Rafael
     * @access public
     * @return select com todos tipos de equipamento
     */
    public function montaComboTipoEquipamento(){

        $sql = "select tpeid as codigo
                      ,tpedsc as descricao
                  from sca.tipoequipamento
                 order by 2";

        $this->monta_combo("tpeid", $sql, 'S' , 'Selecione...','','','','175','S','tpeid');
    }

    /**
     * Método responsável por montar a combo de marca de equipamento
     *
     * @name montaComboMarcaEquipamento
     * @author Alysson Rafael
     * @access public
     * @return select com todas marcas de equipamento
     */
    public function montaComboMarcaEquipamento(){

        $sql = "select mceid as codigo
                      ,mcedsc as descricao
                  from sca.marcaequipamento
                 order by 2";

        $this->monta_combo("mceid", $sql, 'S' , 'Selecione...','','','','175','S','mceid');
    }

    public function salvarEntradaEquipamento($vttid, $nu_matricula_siape, $equipamentos, $usucpf, $imagem = ''){
        $nu_matricula_siape = empty($nu_matricula_siape)    ? null : $nu_matricula_siape;
        $vttid              = empty($vttid)                 ? null : $vttid;
        
        for($i = 0; $i < count($equipamentos); $i++){
            $eqmid = $equipamentos[$i];
            if(!empty($eqmid)){
                $sql = "insert into sca.movimentacaoequipamento ";
                if(!empty($vttid)){
                    $sql .= "(eqmid,vttid,usucpfentrada)
                             values($eqmid,$vttid,'$usucpf') ";
                    $insertFoto = array("vttid" => $vttid);
                }else {                    
                    $sql .= "(eqmid,nu_matricula_siape,usucpfentrada)
                             values($eqmid,$nu_matricula_siape,'$usucpf') ";

                    $insertFoto = array("nu_matricula_siape" => $nu_matricula_siape);
                }

                $sql .= " returning mveid ";
                $mveid = $this->pegaUm($sql);
            }

            if(!empty($imagem)){
                $foto = new FilesSimec("visitantefoto", $insertFoto, "sca");
                $foto->setStream("Foto de visitante", $imagem);
            }
        }

        $this->commit();
    }

    public function salvarSaidaEquipamento($vttid, $nu_matricula_siape, $equipamentos, $usucpf, $imagem = ''){
        
        $nu_matricula_siape = empty($nu_matricula_siape) ? 'null' : $nu_matricula_siape;
        $dataAtual = date("Y-m-d H:i").':00';

        for($i = 0; $i < count($equipamentos); $i++){
            $sql = "update sca.movimentacaoequipamento
                       set mvedatsaida = '$dataAtual'
                          ,usucpfsaida = '$usucpf'
                     where mveid = $equipamentos[$i]";
            

            $this->executar($sql);

            if(!empty($imagem)){
                if(!empty($vttid)){
                    $insertFoto = array("vttid" => $vttid);
                }
                else if(!empty($nu_matricula_siape)){
                    $insertFoto = array("nu_matricula_siape" => $nu_matricula_siape);
                }

                $foto = new FilesSimec("visitantefoto", $insertFoto, "sca");
                $foto->setStream("Foto de visitante", $imagem);
            }
        }

        $this->commit();
    }






    /**
     * método responsável por carregar um equipamento pelo id
     *
     * @name carregaEquipamentoPorId
     * @author Alysson Rafael
     * @access public
     * @param $eqmid id do equipamento
     * @return grid com os dados encontrados
     */
    public function carregaEquipamentoPorId($eqmid){
    	
    	
        $sql = "select e.eqmid
                      ,e.tpeid
                      ,t.tpedsc
                      ,e.mceid
                      ,m.mcedsc
                      ,e.eqmnumserie
                      ,e.eqmnumetiqueta
                      ,e.usucpf
                      ,e.eqmdatcadastro
	                  ,(select 
							(CASE WHEN count(m.mveid)>0 THEN 'Est&aacute dentro do MEC' ELSE 'Est&aacute fora do MEC' end) AS statussaida
							from sca.movimentacaoequipamento m
							where  (m.eqmid = e.eqmid) and m.mvedatsaida IS NULL)  AS statussaida
							
					  ,(SELECT coalesce(visitante.vttnome,vwservidorativo.no_servidor) FROM sca.movimentacaoequipamento meq
						LEFT JOIN sca.visitante visitante ON meq.vttid = visitante.vttid
						LEFT JOIN sca.vwservidorativo vwservidorativo ON meq.nu_matricula_siape = vwservidorativo.nu_matricula_siape
						
						where meq.eqmid = e.eqmid order by meq.mveid desc limit 1)  AS nomeresponsavel
					
                  from sca.equipamento e
                 inner join sca.marcaequipamento m
                    on e.mceid = m.mceid
                 inner join sca.tipoequipamento t
                    on e.tpeid = t.tpeid
				
                 where e.eqmid = $eqmid";
        

        return $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
        //ver($sql);
    }

    /**
     * método responsável por salvar um equipamento
     *
     * @name salvarEquipamento
     * @author Alysson Rafael
     * @access public
     * @param $post dados do form
     * @param $onde se a chamada de cadastro de equipamento é popup ou não
     * @return void
     */
    public function salvarEquipamento($post,$onde){

        $mensagem = "Operação realizada com sucesso!";

        extract($post);

           //caso seja para cadastrar um novo tipo de equipamento ou nova marca
        if($chkNovoTipo == 'on'){
              $sqlInsereTipo = "INSERT INTO sca.tipoequipamento(tpedsc) VALUES('".$txtNovoTipo."') RETURNING tpeid ";
              $tpeid = $this->pegaUm($sqlInsereTipo);
        }
        if($chkNovaMarca == 'on'){
              $sqlInsereMarca = "INSERT INTO sca.marcaequipamento(mcedsc) VALUES('".$txtNovaMarca."') RETURNING mceid ";
              $mceid = $this->pegaUm($sqlInsereMarca);
        }

        //trata nulo
        $eqmnumetiqueta    = (!empty($eqmnumetiqueta))   ? $eqmnumetiqueta : null;
        $tpeid             = (!empty($tpeid))            ? $tpeid          : null;
        $mceid             = (!empty($mceid))            ? $mceid          : null;
        $eqmnumserie       = (!empty($eqmnumserie))      ? $eqmnumserie    : null;

        $sql = '';

        if( $eqmid == '' || empty($eqmid) ){
            
            $eqmnumetiqueta = strtoupper( uniqid() );

            while($this->verificaEtiquetaRepetida($eqmnumetiqueta)){
                $eqmnumetiqueta = strtoupper(uniqid());
            }

            $sqlEquipamento = "select
                                    eqmid
                                from
                                    sca.equipamento
                                where
                                    tpeid={$tpeid}
                                and
                                    mceid={$mceid}
                                and
                                    eqmnumserie = '{$eqmnumserie}' ";

            $eqmid = $this->pegaUm($sqlEquipamento);
            
            if(!$eqmid){

                $sql = 'INSERT INTO sca.equipamento(tpeid,mceid,eqmnumserie,eqmnumetiqueta,usucpf) ';
                $sql.= "VALUES(".$tpeid.",".$mceid.",'".$eqmnumserie."','".$eqmnumetiqueta."','".$_SESSION['usucpf']."')";
                $sql.= " returning eqmid";
                $eqmid = $this->pegaUm($sql);
                $this->commit();
                $imprimir = true;

            }else{

                $imprimir = false;
                $mensagem = "Este equipamento já está gravado!";
            }
        
        }else{
            
            $sql = "UPDATE sca.equipamento set tpeid={$tpeid}, mceid={$mceid},";
            $sql.= " eqmnumserie ='{$eqmnumserie}',";
            $sql.= " usucpf='{$_SESSION['usucpf']}' ";
            $sql.= " WHERE eqmid=".$eqmid;
            $sql.= " returning eqmid";
            $eqmid = $this->pegaUm($sql);
            $this->commit();
            $imprimir = false;
        }

        if($onde == 'popup'){
            direcionar('?modulo=principal/equipamento/popup/cadastrarEquipamento&acao=A&eqmid='.$eqmid.($imprimir?'&imprimir=1':''),$mensagem);
        }else{
            direcionar('?modulo=principal/equipamento/cadastrarEquipamento&acao=A&eqmid='.$eqmid,'Operação realizada com sucesso!');
        }
    }

    /**
     * método responsável por listar equipamentos através dos filtros informados na consulta de equipamentos
     *
     * @name listarEquipamentoPorFiltro
     * @author Alysson Rafael
     * @access public
     * @param $eqmnumetiqueta etiqueta do equipamento
     * @param $eqmnumserie número de série do equipamento
     * @return grid com os dados encontrados
     */
    public function listarEquipamentoPorFiltro( $dados = null ){
        
        $eqmnumetiqueta  = !empty($dados['eqmnumetiqueta'])    ? $dados['eqmnumetiqueta']   : null;
        $eqmnumserie     = !empty($dados['eqmnumserie'])       ? $dados['eqmnumserie']      : null;
        $eqstatus        = !empty($dados['eqstatus'])          ? $dados['eqstatus']         : null;
        $nomeresponsavel = !empty($dados['responsavel_nome'])  ? $dados['responsavel_nome']  : null;
        
        $sisid = $_SESSION['sisid'];
        $where = " where 1=1 ";
        $having = '';
        
        if(!empty($eqmnumetiqueta)){
            $where .= " and equipamento.eqmnumetiqueta = '".$eqmnumetiqueta."'";
        }
        
        if(!empty($eqmnumserie)){
            $where .= " and equipamento.eqmnumserie = '".$eqmnumserie."'";
        }

        if(!empty($eqstatus)){
            if ($eqstatus == 'volvo' ) {
               $having = "HAVING count(status_equip.total)>0";
            }else {
               $having = "HAVING count(status_equip.total)<=0";
            }
        	//$where .= " and equipamento.status = '".$eqstatus."'";
        }

        if( !empty($nomeresponsavel) ){
        	$where .= " and responsavel.nome ilike '%".$nomeresponsavel."%'";
        }
/*        
        $sql = "SELECT '<center>
                    <a style=\"cursor:pointer;\" onclick=\"detalharEquipamento(\''||equipamento.eqmid||'\');\">
                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                    </a>

                    </center>'
                                                                         
 as acao, --equipamento.eqmid,
                    tipoequipamento.tpedsc||' ' as tpedsc,
                    marcaequipamento.mcedsc||' ' as mcedsc,
                    equipamento.eqmnumserie||' ' as eqmnumserie,
                    responsavel.nome|| ' ' as resposnsavel,
                    
                    (select 
						(CASE WHEN count(m.mveid)>0 THEN 'Está dentro do MEC' ELSE 'Está fora do MEC' end) AS statussaida
						from sca.movimentacaoequipamento m
						where  (m.eqmid = equipamento.eqmid) and m.mvedatsaida IS NULL)  AS statussaida,
						
					  responsavel.nome AS nomeresponsavel
						
                FROM sca.equipamento equipamento
                JOIN sca.tipoequipamento tipoequipamento ON (equipamento.tpeid = tipoequipamento.tpeid)
                JOIN sca.marcaequipamento marcaequipamento ON (equipamento.mceid = marcaequipamento.mceid)
                left join (SELECT distinct meq.eqmid,  coalesce(visitante.vttnome,vwservidorativo.no_servidor) as nome 
				FROM sca.movimentacaoequipamento meq
				LEFT JOIN sca.visitante visitante ON meq.vttid = visitante.vttid
				LEFT JOIN sca.vwservidorativo vwservidorativo ON meq.nu_matricula_siape = vwservidorativo.nu_matricula_siape
) as responsavel on equipamento.eqmid = responsavel.eqmid

                 -- where 1=1 
                   ". $where."
            -- ORDER BY equipamento.eqmid
            ORDER BY tipoequipamento.tpedsc ";
        
      
        
//        $sql = "SELECT '[Alterar]'as acao,tipoequipamento.tpedsc||' ' as tpedsc
//	, marcaequipamento.mcedsc||' ' as mcedsc
//	, equipamento.eqmnumserie||' ' as eqmnumserie
//	, responsavel.nome|| ' ' as resposnsavel
//	, CASE 
//	    WHEN COALESCE(status_equip.total, 0 )  > 0 THEN 'Está dentro do MEC' 
//            ELSE 'Está fora do MEC'
//	  END AS statussaida
//	, responsavel.nome AS nomeresponsavel
//						
//FROM sca.equipamento equipamento
//JOIN sca.tipoequipamento tipoequipamento 
//	ON (equipamento.tpeid = tipoequipamento.tpeid)
//INNER JOIN sca.marcaequipamento marcaequipamento 
//	ON (equipamento.mceid = marcaequipamento.mceid)
//LEFT JOIN ( 
//	   SELECT distinct meq.eqmid,  coalesce(visitante.vttnome,vwservidorativo.no_servidor) as nome 
//	   FROM sca.movimentacaoequipamento meq
//	   LEFT JOIN sca.visitante visitante ON meq.vttid = visitante.vttid
//	   LEFT JOIN sca.vwservidorativo vwservidorativo ON meq.nu_matricula_siape = vwservidorativo.nu_matricula_siape
//	   ) as responsavel 
//	   ON equipamento.eqmid = responsavel.eqmid
//LEFT JOIN 
//	(
//	SELECT m.eqmid, count( m.mveid ) as total
//	FROM sca.movimentacaoequipamento m
//	WHERE m.mvedatsaida IS NULL
//	GROUP BY m.eqmid
//	) AS status_equip
//	ON equipamento.eqmid =  status_equip.eqmid	
// ". $where."
//ORDER BY tipoequipamento.tpedsc ";
//
//
        //ver($where);
  */      
    $sql ="SELECT DISTINCT '<center>
                <a style=\"cursor:pointer;\" onclick=\"detalharEquipamento(\''||equipamento.eqmid||'\');\">
                    <img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
                </a>
                </center>'

            as acao, 
            tipoequipamento.tpedsc||' ' as tpedsc,
            marcaequipamento.mcedsc||' ' as mcedsc,
            equipamento.eqmnumserie||' ' as eqmnumserie,
            
            CASE  WHEN COALESCE(status_equip.total, 0 )  > 0 THEN 'Está dentro do MEC' 
                ELSE 'Está fora do MEC'
            END AS statussaida, 
            responsavel.nome AS nomeresponsavel
			
        FROM sca.equipamento equipamento
        JOIN sca.tipoequipamento tipoequipamento 
            ON (equipamento.tpeid = tipoequipamento.tpeid)
        INNER JOIN sca.marcaequipamento marcaequipamento 
            ON (equipamento.mceid = marcaequipamento.mceid)
        LEFT JOIN ( 
            SELECT distinct meq.eqmid,  coalesce(visitante.vttnome,vwservidorativo.no_servidor) as nome 
                FROM sca.movimentacaoequipamento meq
                LEFT JOIN sca.visitante visitante ON meq.vttid = visitante.vttid
                LEFT JOIN sca.vwservidorativo vwservidorativo ON meq.nu_matricula_siape = vwservidorativo.nu_matricula_siape
            ) as responsavel 
            ON equipamento.eqmid = responsavel.eqmid
        LEFT JOIN ( SELECT m.eqmid, count( m.mveid ) as total
                    FROM sca.movimentacaoequipamento m
                    WHERE m.mvedatsaida IS NULL
                    GROUP BY m.eqmid ) AS status_equip
    	ON equipamento.eqmid =  status_equip.eqmid	
        ". $where."
        GROUP BY acao, tpedsc, mcedsc, eqmnumserie, statussaida, nomeresponsavel
        ". $having."
        ORDER BY tpedsc ";
       
        $cabecalho = array("Ação","Tipo","Marca","Número de Série","Status","Responsável");
        $this->monta_lista($sql,$cabecalho,100,50,false,"center");
    }

    /**
     * método responsável por excluir um equipamento
     *
     * @name excluirEquipamento
     * @author Alysson Rafael
     * @access public
     * @param $eqmid id do equipamento
     * @return void
     */
    public function excluirEquipamento($eqmid){

        $this->executar("DELETE FROM sca.equipamento WHERE eqmid ='".$eqmid."'");
        $this->commit();
        direcionar('?modulo=principal/equipamento/consultarEquipamento&acao=A','Operação realizada com sucesso!');
    }

    /**
     * método que verifica se existe algum equipamento cadastrado com o número da etiqueta informado
     *
     * @name verificaEtiquetaRepetida
     * @author Alysson Rafael
     * @access public
     * @param $valor número da etiqueta
     * @return $eqmid integer id do equipamento
     */
    public function verificaEtiquetaRepetida($valor){

        $sql = "SELECT equipamento.eqmid
                  FROM sca.equipamento equipamento
                 WHERE equipamento.eqmnumetiqueta = '$valor'";

            return $this->pegaUm($sql);
    }

    public function verificaEntradaSemSaida($eqmid){
        $sql = "select count(*) as qtd
                  from sca.movimentacaoequipamento
                 where eqmid = $eqmid
                   and mvedatsaida is null";

        return ($this->pegaUm($sql) > 0);
    }


    /**
     * método responsável por recuperar a etiqueta de um equipamento
     *
     * @name recuperaEtiqueta
     * @author Alysson Rafael
     * @access public
     * @param $eqmid id do equipamento
     * @return $eqmnumetiqueta integer número da etiqueta
     */
    public function recuperaEtiqueta($eqmid){

        $sql = "SELECT equipamento.eqmnumetiqueta FROM sca.equipamento equipamento WHERE equipamento.eqmid='".$eqmid."' ";
        $result = $this->carregar($sql);
            $eqmnumetiqueta = $result[0]['eqmnumetiqueta'];
            return $eqmnumetiqueta;

    }

    /**
     * método responsável por verificar se um equipamento está sem registro de entrada.
     * Verificação feita ao tentar registrar a saída
     *
     * @name verificaSaidaSemEntrada
     * @author Alysson Rafael
     * @access public
     * @param $equipamento id do equipamento
     * @return $qtd integer count da consulta sql
     */
    public function verificaSaidaSemEntrada($equipamento){
        $sql = "SELECT COUNT(*) AS qtd FROM sca.movimentacaoequipamento movimentacao ";
        $sql .= "WHERE movimentacao.eqmid='".$equipamento."' AND movimentacao.mvedatentrada IS NULL ";

        $result = $this->carregar($sql);
            $qtd = $result[0]['qtd'];
            return $qtd;
    }





    /**
     * Método responsável por montar a combo do equipamento
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

        combo_popup( 'eqmid[]', $sql, 'Selecione o(s) Equipamento(s)', '360x460', 0,array(), '', 'S', false, false, 10, 400, null, null, false, null, $items, true,false, null, false, null, null);
    }
}
