<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once "dompdf-master/dompdf_config.inc.php";
include_once "dompdf-master/include/dompdf.cls.php";


/*************************************************************************************
* Config Etiqueta
*************************************************************************************/
define("LARGURA_ETIQUETA_SCA", 290);
define("ALTURA_ETIQUETA_SCA", 100);
define('ETIQUETAS_TEMPLATES_PATH', APPRAIZ.'sca/modulos/principal/visitante/etiquetasTemplates/');
define('TEMPLATE', ETIQUETAS_TEMPLATES_PATH.'novo_template.png');
define('TXT_FONT', ETIQUETAS_TEMPLATES_PATH.'Arial.ttf');


class Visitante extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sca.visitante";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vttid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array('vttid' => null,
                                       'tidid' => null,
                                       'ogeid' => null,
                                       'arqid' => null,
                                       'vttnome' => null,
                                       'vttobs' => null,
                                       'vtttel'=>null,
                                       'vttdoc'=>null,
                                       'usucpf'=>null,
                                       'vttdatcadastro'=>null
                                      );

    public function recuperarIdFoto($vttid, $nu_matricula_siape){

        $sql = "select arqid
                  from sca.visitantefoto";

        if($vttid && !empty($vttid)){
            $sql .= " where vttid = $vttid";
        } else{
            $sql .= " where nu_matricula_siape = $nu_matricula_siape";
        }
        $sql .= " ORDER BY svfid DESC LIMIT 1"; // Alterado para trazer sempre o ultimo arquivo salvo

        try {
        	$retorno = $this->pegaUm($sql);
        	return $retorno;
        } catch (Exception $e) {
        	echo "<pre>";
        	var_dump($e);
        	exit;
        }

    }

    public function recuperarFotoVisitante($vttid, $nu_matricula_siape){

        $idArquivo = null;

        if(($vttid && !empty($vttid)) || ($nu_matricula_siape && !empty($nu_matricula_siape))){
            $sql = "select arqid
                      from sca.visitantefoto";

            if($vttid && !empty($vttid)){
                $sql .= " where vttid = $vttid";
            } else{
                $sql .= " where nu_matricula_siape = $nu_matricula_siape";
            }

            $idArquivo = $this->pegaUm($sql);
        }

        if($idArquivo){
            $foto = new FilesSimec("visitantefoto", array() ,"sca");
            if($foto->existeArquivo($idArquivo)){
                echo "<img src='?modulo=principal/imagem&acao=A&idArquivo=$idArquivo' alt='Foto Visitante' id='imagemVisitante' width='320px' height='240px' />";



            }
        }
        else {
        echo "<object classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000'
                id='CapturaFoto' width='320' height='240'
                codebase='http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab'>
                <param name='movie' value='componentes/CapturaFoto.swf' />
                <param name='quality' value='high' />
                <param name='bgcolor' value='#ffffff' />
                <param name='allowScriptAccess' value='sameDomain' />
                <embed src='componentes/CapturaFoto.swf' quality='high' bgcolor='#ffffff'
                    width='320' height='240' name='CapturaFoto' align='middle'
                    play='true' loop='false' quality='high' allowScriptAccess='sameDomain'
                    type='application/x-shockwave-flash'
                    pluginspage='http://www.adobe.com/go/getflashplayer'>
                </embed>
              </object>";
        }

    }


    public function editarFotoVisitante($vttid, $nu_matricula_siape){

        $idArquivo = null;

        if(($vttid && !empty($vttid)) || ($nu_matricula_siape && !empty($nu_matricula_siape))){
            $sql = "select arqid
                      from sca.visitantefoto";

            if($vttid && !empty($vttid)){
                $sql .= " where vttid = $vttid";
            } else{
                $sql .= " where nu_matricula_siape = $nu_matricula_siape";
            }

            $idArquivo = $this->pegaUm($sql);
        }

        if($idArquivo){
            $foto = new FilesSimec("visitantefoto", array() ,"sca");
            if($foto->existeArquivo($idArquivo)){
                echo "<img src='?modulo=principal/imagem&acao=A&idArquivo=$idArquivo' alt='Foto Visitante' id='imagemVisitante' width='320px' height='240px' />";



            }
        }

        echo "<object classid='clsid:D27CDB6E-AE6D-11cf-96B8-444553540000'
                id='CapturaFoto' width='320' height='240'
                codebase='http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab'>
                <param name='movie' value='componentes/CapturaFoto.swf' />
                <param name='quality' value='high' />
                <param name='bgcolor' value='#ffffff' />
                <param name='allowScriptAccess' value='sameDomain' />
                <embed src='componentes/CapturaFoto.swf' quality='high' bgcolor='#ffffff'
                    width='320' height='240' name='CapturaFoto' align='middle'
                    play='true' loop='false' quality='high' allowScriptAccess='sameDomain'
                    type='application/x-shockwave-flash'
                    pluginspage='http://www.adobe.com/go/getflashplayer'>
                </embed>
              </object>";


    }

    /**
     * método responsável por pesquisar pessoa pelos filtros informados
     *
     * @name listarVisitantePorFiltro
     * @author Alysson Rafael
     * @access public
     * @param $vttdoc documento da pessoa a ser pesquisada
     * @param $vttnome nome da pessoa a ser pesquisada
     * @param $no_unidade_org string nome da unidade a ser pesquisada
     * @param $tipoConsulta indica se a consulta é apenas por servidor, apenas visitante ou ambos
     * @return grid com os dados encontrados
     */
    public function listarVisitantePorFiltro($vttdoc, $vttnome, $no_unidade_org, $tipoConsulta){

        if($tipoConsulta == 'V' || $tipoConsulta == 'T' || $tipoConsulta == 'A'){
            $where = " where 1=1 ";
            if(!empty($vttdoc)){
                $where .= " and v.vttdoc = '$vttdoc'";
            }
            if(!empty($vttnome)){
                $where .= " and v.vttnome ilike '%$vttnome%'";
            }
            if(!empty($no_unidade_org)){
                $where .= " and exists (select a.autid
                                          from sca.autorizado a
                                         inner join sca.autorizacao c
                                            on a.autid = c.autid
                                         inner join sca.vwunidadeorganizacional u
                                            on c.co_interno_uorg = u.co_interno_uorg
                                         where a.vttid = v.vttid
                                           and u.no_unidade_org ilike '%$no_unidade_org%')";
                $uorg = "(select u.no_unidade_org
                            from sca.autorizado a
                           inner join sca.autorizacao c
                              on a.autid = c.autid
                           inner join sca.vwunidadeorganizacional u
                              on c.co_interno_uorg = u.co_interno_uorg
                           where a.vttid = v.vttid
                             and u.no_unidade_org ilike '%$no_unidade_org%'
                           limit 1)";

            }
            else
                $uorg = "''";

            $sql = "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(\'' || v.vttid || '\', null, \'' || replace(v.vttnome, '\'', chr(92)||chr(39)) || '\');\">
                            <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>
                            </center>' as acao
                          ,v.vttdoc||' ' as vttdoc
                          ,td.tiddsc||' ' as tiddsc
                          ,v.vttnome||' ' as vttnome
                          ,case when length(v.vtttel) = 10
                                then '(' || substr(v.vtttel,1,2) || ')' || substr(v.vtttel,3,4) || '-' || substr(v.vtttel,7,4)
                                else case when length(v.vtttel) = 8
                                          then substr(v.vtttel,1,4) || '-' || substr(v.vtttel,5,4)
                                          else v.vtttel || ' '
                                     end
                           end as vtttel
                           ,$uorg as no_unidade_org
                      FROM sca.visitante v
                      INNER JOIN sca.tipodocumento td ON v.tidid = td.tidid
                    $where";

            //ver($sql,d);
        }

        if($tipoConsulta == 'T' || $tipoConsulta == 'A'){
            $sql .= " UNION ";
        }

        if($tipoConsulta == 'S' || $tipoConsulta == 'T' || $tipoConsulta == 'A'){
            $where = " where 1=1 ";
            if(!empty($vttdoc)){
                $where .= " and s.nu_cpf = '$vttdoc'";
            }
            if(!empty($vttnome)){
                $where .= " and s.no_servidor ilike '%$vttnome%'";
            }
            if(!empty($no_unidade_org)){
                $where .= " and s.no_unidade_org ilike '%$no_unidade_org%'";
            }


            $sql .= "SELECT '<center><a style=\"cursor:pointer;\" onclick=\"selecionarPessoa(null, ' || s.nu_matricula_siape || ', \'' || replace(s.no_servidor, '\'', chr(92)||chr(39)) || '\');\">
                             <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\"></a>
                             </center>' as acao
                           ,s.nu_cpf||' ' as vttdoc
                           ,'Matrícula SIAPE' as tiddsc
                           ,s.no_servidor||' ' as vttnome
                           ,'' as vtttel
                           ,no_unidade_org||' ' as no_unidade_org
                           --s.co_funcao , s.co_orgao
                           --s.*
                       FROM sca.vwservidorativo s
                      
                     $where";
          // ver($sql);
        }

        $sql .= " ORDER BY vttnome";
        $cabecalho = array("Ação", "Documento", "Tipo de Documento", "Nome", "Telefone", "Unidade");

        $this->monta_lista($sql,$cabecalho,20,30,false,"center");
    }

/**
     * Método responsável por pesquisar pessoa pelos filtros informados
     *
     * @name listarVisitantes
     * @author Alysson Rafael
     * @access public
     * @param $vttdoc documento da pessoa a ser pesquisada
     * @param $vttnome nome da pessoa a ser pesquisada
     * @param $no_unidade_org string nome da unidade a ser pesquisada
     * @param $tipoConsulta indica se a consulta é apenas por servidor, apenas visitante ou ambos
     * @return grid com os dados encontrados
     */
    public function listarVisitantes($vttdoc, $vttnome, $no_unidade_org, $tipoConsulta){

        extract($_POST);

        $where = " where 1=1 ";
        if(!empty($vttdoc)){
            $where .= " and vttdoc = '$vttdoc'";
        }
        if(!empty($vttnome)){
            $where .= " and vttnome ilike '%$vttnome%'";
        }

        $sql = "SELECT
                    '<center>
                        <a style=\"cursor:pointer;\" onclick=\"editarVisitante(' || vttid || ')\">
                            <img src=\"/imagens/alterar.gif \" border=0 title=\"Selecionar\">
                        </a>
                    </center>' as acao,
                    v.vttdoc,
                    v.vttnome,
                    case when length(v.vtttel) = 10
                        then '(' || substr(v.vtttel,1,2) || ')' || substr(v.vtttel,3,4) || '-' || substr(v.vtttel,7,4)
                        else case when length(v.vtttel) = 8
                                  then substr(v.vtttel,1,4) || '-' || substr(v.vtttel,5,4)
                                  else v.vtttel || ' '
                        end
                    end as vtttel,
                    u.no_unidade_org,
                    CASE WHEN v.vttterceiro = true THEN 'Terceirizado' ELSE 'Visitante' END
                FROM
                    sca.visitante v
                LEFT JOIN
                    sca.vwunidadeorganizacional u
                    ON v.co_uorg = u.co_interno_uorg
                    $where ";

        $cabecalho = array("Ação", "Documento", "Nome", "Telefone", "Unidade", "Tipo");
        $this->monta_lista($sql,$cabecalho,20,30,false,"center");

    }

   public function listarAutorizacoesPorFiltro($vttdoc = NULL, $vttnome = NULL, $status = NULL, $autdatinicio = NULL, $autdatfinal = NULL, $no_unidade_org = NULL, $dtEntradaAut = NULL, $booRetornarRegistro = FALSE ){

        $whereStatusVisitante = "";
        $whereStatusServidor = "";
        $whereVisitante = "";
        $whereServidor = "";
        $whereFiltro = "";
        $destaque = "link";
        if(!empty($vttdoc)){
            $whereServidor  .= " and servidor.nu_cpf ilike '$vttdoc%' ";
            $whereVisitante .= " and visitante.vttdoc ilike '$vttdoc%' ";
            $destaque = "(CASE WHEN strpos(upper(link),upper('$vttdoc')) > 0 THEN '<strong>'||link||'</strong>' else link END) as link";
        }
        if(!empty($vttnome)){
            $whereServidor  .= " and servidor.no_servidor ilike '$vttnome%' ";
            $whereVisitante .= " and visitante.vttnome ilike '$vttnome%' ";

            if(empty($vttdoc)){
                $destaque = "(CASE WHEN strpos(upper(link),upper('$vttnome')) > 0 THEN '<strong>'||link||'</strong>' else link END) as link";
            }
        }

        if( !empty( $autdatinicio ) || !empty( $autdatfinal ) || !empty( $no_unidade_org ) || !empty( $dtEntradaAut ) )
        {
            $wherePrincipal = "where 1 = 1 ";

            if( !empty( $autdatinicio ) ){
                $wherePrincipal = "and autdatinicio = '$autdatinicio' ";
            }

            if( !empty( $autdatfinal ) ){
                $wherePrincipal = "and autdatfinal = '$autdatfinal' ";
            }

            if( !empty( $no_unidade_org ) ){
                $wherePrincipal = "and no_unidade_org ILIKE '%" . pg_escape_string( $no_unidade_org ) . "%' ";
            }

            if( !empty( $dtEntradaAut ) ){
                $wherePrincipal = "and '$dtEntradaAut' BETWEEN autdatinicio AND autdatfinal ";
            }
        }

        if(!empty($vttdoc) || !empty($vttnome)){
            $whereFiltro = "where exists(select autorizado.audid
                                           from sca.visitante visitante
                                           join sca.autorizado autorizado
                                             on visitante.vttid = autorizado.vttid
                                          where autorizado.autid = autorizacao.autid
                                         $whereVisitante
                                          union
                                         select autorizado.audid
                                           from sca.vwservidorativo servidor
                                           join sca.autorizado autorizado
                                             on servidor.nu_matricula_siape = autorizado.nu_matricula_siape
                                          where autorizado.autid = autorizacao.autid
                                         $whereServidor)";
        }

        if(!empty($status)){
            $whereStatusVisitante = "";
            $whereStatusServidor = "";


            switch ($status) {
                // Autorização Permanente
                case '1':
                    $whereStatusVisitante = " and 1 = 2 ";
                    $whereStatusVisitante = " where (current_date >= autorizacao.autdatinicio::date  and autorizacao.autdatfinal::date is null) ";
                    //$whereStatusVisitante = "";
                    break;
                // Autorização Vigente
                case '2':
                    $whereStatusServidor = " and 1 = 2 ";
                    $whereStatusVisitante = " where (current_date between autorizacao.autdatinicio::date  and autorizacao.autdatfinal::date) ";
                    //$whereStatusVisitante = " AND (current_date between autorizacao.autdatinicio::date  and autorizacao.autdatfinal::date) ";
                    break;
                // Autorização Futura
                case '3':
                    $whereStatusServidor = " and 1 = 2 ";
                    $whereStatusVisitante = " where (autorizacao.autdatinicio::date > current_date) ";
                    //$whereStatusVisitante = " AND (autorizacao.autdatinicio::date > current_date) ";
                    break;
                // Autorização Vencida
                case '4':
                    $whereStatusServidor = " and 1 = 2 ";
                    $whereStatusVisitante = "where (current_date > autorizacao.autdatfinal::date)/* Vencida */ ";
                    //$whereStatusVisitante = "AND (current_date > autorizacao.autdatfinal::date)/* Vencida */ ";
                    break;
            }
        }

        $sql = "select legenda, link,
                no_unidade_org, autdatinicio, autdatfinal, nomesolicitante, auttelsolicitante
                  from (select legenda
                                ,$destaque
                                ,no_unidade_org, autdatinicio, autdatfinal, nomesolicitante, auttelsolicitante, vttnome
                            from (select ('<img src=\"../imagens/p_azul.gif\">')::text as legenda
                              ,'<a href=\"sca.php?modulo=principal/visitante/registrarEntradaVisitante&acao=A&nu_matricula_siape=' || servidor.nu_matricula_siape || '\">' || servidor.nu_cpf || ' - ' || servidor.no_servidor || '</a>' as link
                              ,servidor.no_unidade_org
                              ,(null)::varchar as autdatinicio
                              ,(null)::varchar as autdatfinal
                              ,(null)::varchar as nomesolicitante
                              ,(null)::varchar as auttelsolicitante
                              ,servidor.no_servidor as vttnome
                          from sca.vwservidorativo servidor
                         where (servidor.ds_cargo_emprego ilike 'MINISTRO DE ESTADO%'
                            or (servidor.co_funcao = 'DAS'
                           and servidor.co_nivel_funcao in (1014,1015,1016,1024,1025,1026))
                            or (servidor.co_funcao = 'NES')
                            or (servidor.ds_cargo_emprego ilike 'CHEFE DE GABINETE%'))
                           
                        $whereServidor $whereStatusServidor) permanente
                         union
                                -- nao permanentes
				SELECT
                                    case when autorizacao.autdatinicio::date > current_date then '<a href=\"sca.php?modulo=principal/visitante/autorizarAcessoForaHorario&acao=A&autid=' || autorizado.autid || '\"><img src=\"../imagens/p_amarelo.gif\"></a>'
                                    when current_date > autorizacao.autdatfinal::date then '<img src=\"../imagens/p_vermelho.gif\">'
                                    else '<img src=\"../imagens/p_verde.gif\">' end as legenda

                                    ,case when current_date >= autorizacao.autdatinicio::date and current_date <= autorizacao.autdatfinal::date
                                    then '<a href=\"sca.php?modulo=principal/visitante/registrarEntradaVisitante&acao=A&audid=' || autorizado.audid || '\">' || vttdoc || ' - ' || vttnome || '</a>'
                                    else vttdoc || ' - ' || vttnome end as link


                                    ,unidade.no_unidade_org
                                    ,to_char(autorizacao.autdatinicio, 'DD/MM/YYYY')
                                    ,to_char(autorizacao.autdatfinal, 'DD/MM/YYYY')
                                    ,solicitante.no_servidor as nomesolicitante
                                    ,case when length(autorizacao.auttelsolicitante) = 10 then '(' || substr(autorizacao.auttelsolicitante,1,2) || ')' || substr(autorizacao.auttelsolicitante,3,4) || '-' || substr(autorizacao.auttelsolicitante,7,4)
                                    when length(autorizacao.auttelsolicitante) = 8 then substr(autorizacao.auttelsolicitante,1,4) || '-' || substr(autorizacao.auttelsolicitante,5,4)
                                    else autorizacao.auttelsolicitante end as auttelsolicitante
                                    ,(null)::varchar as vttnome

				FROM

				    (-- visitante
				    SELECT
					  visitante.vttdoc,
					  visitante.vttnome,
					  autid, audid
				    FROM
					    sca.visitante visitante
				    INNER JOIN sca.autorizado
					    ON visitante.vttid = autorizado.vttid
				     $whereVisitante 

					UNION

				    -- servidor
				    SELECT
					    servidor.nu_cpf as vttdoc,
					    servidor.no_servidor as vttnome,
					    autid, audid
				    FROM
					    sca.vwservidorativo servidor
				    INNER JOIN sca.autorizado
					    ON servidor.nu_matricula_siape = autorizado.nu_matricula_siape
				     $whereServidor
                                    ) autorizado

				    INNER JOIN
					    sca.autorizacao
					    ON autorizado.autid=autorizacao.autid
				    INNER JOIN sca.vwservidorativo solicitante
					    ON autorizacao.nu_matricula_siape = solicitante.nu_matricula_siape
				    LEFT JOIN sca.vwunidadeorganizacional unidade
					    ON autorizacao.co_interno_uorg = unidade.co_interno_uorg
                                    $whereStatusVisitante
                ) as t1
                where 1 = 1
                $wherePrincipal
                order by case when autdatfinal is null
                                then 1
                                else case when to_date(autdatinicio, 'DD/MM/YYYY') > current_date then 3
                                         when current_date > to_date(autdatfinal, 'DD/MM/YYYY') then 4
                                         else 3
                                    end
                          end
                         ,to_date(autdatinicio, 'DD/MM/YYYY')
                         ,vttnome";


        $cabecalho = array("Autorização","Nome","Unidade","Data de Início","Data Final","Solicitante","Telefone Solicitante");

        if( $booRetornarRegistro === TRUE )
        {
            return $this->pegaLinha( $sql );
        }
        else
        {
            $this->monta_lista($sql,$cabecalho,20,50,false,"center");
        }
    }

    public function listarAutorizados($vttdoc, $vttnome){

        $whereVisitante = "";
        $whereServidor = "";
        $chave = "";

        if(!empty($vttdoc)){
            $whereServidor  .= "where servidor.nu_cpf ilike '$vttdoc%'";
            $whereVisitante .= "where visitante.vttdoc ilike '$vttdoc%'";
            $chave = "vttdoc";
        }
        else{
            $whereServidor  .= "where servidor.no_servidor ilike '$vttnome%'";
            $whereVisitante .= "where visitante.vttnome ilike '$vttnome%'";
            $chave = "vttnome";
        }

        $sql = "select distinct vttdoc, vttnome
                  from ((select distinct visitante.vttdoc, visitante.vttnome
                           from sca.visitante visitante
                         $whereVisitante
                          limit 10)
                          union
                        (select servidor.nu_cpf as vttdoc, servidor.no_servidor as vttnome
                           from sca.vwservidorativo servidor
                         $whereServidor
                          limit 10)) as t1
                 order by $chave
                 limit 10";

        $autorizados = $this->carregar($sql);
        $autorizados = ($autorizados) ? $autorizados : array();
        $resultado = "";

        foreach ($autorizados as $key => $value){
                $item = trim($value[$chave]);
                $resultado .= "{$item}|{$item}\n";
        }

        return $resultado;
    }

    /**
     * Método responsável por montar a combo do tipo do documento do visitante
     *
     * @name montaComboTipoDocumento
     * @author Cézar Cirqueira
     * @access public
     * @return select com todos tipos de documento ativos
     */
    public function montaComboTipoDocumento(){
        $sql = "SELECT tidid as codigo, tiddsc as descricao
                  FROM sca.tipodocumento
                  WHERE tidstatus = 'A'
                  ORDER BY 2";

        $resultado = $this->carregar($sql);

        $this->monta_combo("tidid",$resultado,'S',"Selecione...","tratarTipoDocumento","","","200","S","tidid","",$tidid);
    }

    /**
     * Método responsável por montar a combo do órgão emissor do documento do visitante
     *
     * @name montaComboOrgaoEmissor
     * @author Cézar Cirqueira
     * @access public
     * @return select com todos órgãos emissores ativos
     */
    public function montaComboOrgaoEmissor(){
        $sql = "SELECT ogeid as codigo, ogedsc as descricao
                  FROM sca.orgaoemissor
                  WHERE ogestatus = 'A'
                  ORDER BY 2";

        $resultado = $this->carregar($sql);

        $this->monta_combo("ogeid",$resultado,'S',"Selecione...","","","","200","S","ogeid","",$ogeid);

    }

    /**
     * Método responsável por buscar o visitante por id
     *
     * @name carregaVisitantePorId
     * @author Cézar Cirqueira
     * @access public
     * @return visitante
     */
    public function carregaVisitantePorId($vttid){
        $mensagem = "";
        $sql = "select *
                      ,substring(vtttel, 1, 2) vttdddtel
                      ,substring(vtttel, 3, 4) || '-' || substring(vtttel, 7, 4) as vtttel
                  from sca.visitante
                 where vttid = $vttid;";

        return $this->pegaLinha( iconv( "UTF-8", "ISO-8859-1", $sql) );
    }

    /**
     * Método responsável por verificar se o visitante já foi cadastrado
     *
     * @name verificaVisitanteCadastrado
     * @author Cézar Cirqueira
     * @access public
     * @return mensagem do sucesso ou fracasso
     */
    public function verificaVisitanteCadastrado($vttid, $vttdoc, $tidid, $ogeid){
        $mensagem = '';
        $vttdoc = preg_replace('/[^A-Z0-9]/', '', strtoupper($vttdoc));

        if(!empty($vttdoc)){
            $sql = "SELECT count(*)
                      FROM sca.visitante
                     WHERE vttdoc = '$vttdoc'
                       AND tidid = $tidid
                       AND ogeid = $ogeid";

            if(!empty($vttid) || $vttid != ''){
                $sql .= " AND vttid != $vttid";
            }

            $resultado = $this->pegaUm($sql);

            if($resultado > 0){
                $mensagem = 'Já existe um visitante cadastrado com este documento!';
            }
        }

        return $mensagem;
    }

    /**
     * Método responsável por cadastrar o visitante
     *
     * @name salvarVisitante
     * @author Cézar Cirqueira
     * @access public
     * @return visitante
     */
    public function salvarVisitante($_POSTDADOS, $usucpf, $imagemVisitante, $alteracaoTipo = false){
    	extract($_POSTDADOS);
        $sql = "";

        $tidid = (!empty($tidid)) ? $tidid : 'null';
        $ogeid = (!empty($ogeid)) ? $ogeid : 'null';
        $arqid = (!empty($arqid)) ? $arqid : 'null';
        $vttnome =(!empty($vttnome)) ? $vttnome : '';
        $vttobs = (!empty($vttobs)) ? $vttobs : '';
        $vttdddtel = (!empty($vttdddtel)) ? $vttdddtel : '';
        $vtttel = (!empty($vtttel)) ? $vtttel : '';
        $vttdoc = (!empty($vttdoc)) ? $vttdoc : '';

        $vttnome = strtoupper(trim($vttnome));
        $vtttel = preg_replace('/[^0-9]/', '', $vttdddtel . $vtttel);
        $vttobs = trim($vttobs);
        $vttdoc = preg_replace('/[^A-Z0-9]/', '', strtoupper($vttdoc));

        if(!empty($co_interno_uorg)){

            $updateUorg = " ,co_uorg=$co_interno_uorg ";
            $insertNameUorg = " ,co_uorg ";
            $insertValueUorg = " ,$co_interno_uorg ";

        }else{

            $updateUorg = " ,co_uorg=null ";
            $insertNameUorg = " ,co_uorg ";
            $insertValueUorg = " ,null ";

        }

        if(!empty($vttterceiro)){

            $updateTerc = " ,vttterceiro=true ";
            $insertNameTerc = " ,vttterceiro ";
            $insertValueTerc = " ,true ";

        }else{

            $updateTerc = " ,vttterceiro=false ";
            $insertNameTerc = " ,vttterceiro ";
            $insertValueTerc = " ,false ";

        }

        if(!empty($vttid)){
            $dataAtual=date("Y-m-d H:i:s");

            $sql = "UPDATE sca.visitante
                       SET tidid = $tidid
                          ,ogeid = $ogeid
                          ,vttnome = '$vttnome'
                          ,vttobs = '$vttobs'
                          ,vtttel = '$vtttel'
                          ,vttdoc = '$vttdoc'
                          ,usucpf = '$usucpf'
                          ,vttdatcadastro = '$dataAtual'
                          $updateUorg
                          $updateTerc
                     WHERE vttid = $vttid";

        }else{
            $sql = "INSERT
                    INTO sca.visitante(tidid, ogeid, vttnome, vttobs, vtttel, vttdoc, usucpf $insertNameUorg $insertNameTerc)
                    VALUES ($tidid,
                            $ogeid,
                            '$vttnome',
                            '$vttobs',
                            '$vtttel',
                            '$vttdoc',
                            '$usucpf'
                            $insertValueUorg
                            $insertValueTerc)";
        }

        $sql .=" returning vttid";
        $itemSalvo = $this->pegaUm($sql);

        if(!empty($imagemVisitante)){

        	try {
        		$sqlDelete = "delete from sca.visitantefoto where vttid = " . $itemSalvo;

        		if( $this->executar( $sqlDelete ) ){

        			$insertFoto = array("vttid" => $itemSalvo);

        			$foto = new FilesSimec("visitantefoto", $insertFoto ,"sca");
        			$idArquivo = $foto->setStream("Foto de visitante", $imagemVisitante);

        			$this->commit();

        		}else{
        			throw new Exception( "Não foi possível excluir a foto." );
        		}

        	} catch (Exception $e) {
        		$this->rollback();
        	}

        }

        echo "<script language='javascript' type='text/javascript' src='./js/sca.js'></script>";

        if($alteracaoTipo){

            echo "<script>
                        alert('Operação realizada com sucesso!');
                        window.opener.location = '?modulo=principal/visitante/atualizarVisitante&acao=A&pesquisar=filtrar'
                        self.close();
                  </script>";

        }else{

            echo "<script>
                      alert('Operação realizada com sucesso!');
                      executarScriptPai('carregarPessoa($itemSalvo,null);');
                      self.close();
                  </script>";
        }

        $this->commit();
    }

	public function geraEtiqueta( $dados ){


		if (empty ( $dados ['vsnumcrachasistema'] )) {
			return false;
		}

		$key = md5 ( 'etiqueta_brasao' );
		$pathImg = APPRAIZ . '/www/imagens/brasao.png';

		$imgBrasao = $this->getImagem ( $key, $pathImg );

		$key = md5 ( 'etiqueta_semfoto' );
		$pathImg = APPRAIZ . '/www/imagens/semfoto.jpg';
		$imgFotoPadrao = $this->getImagem ( $key, $pathImg );



		$sql = "SELECT 
		        (SELECT edfdsc FROM sca.edificio WHERE edfid={$dados['edfid']}) as edificio, 
			    (SELECT edadsc FROM sca.edificioandar WHERE edaid={$dados['edaid']}) as andar,
			    (SELECT dstsigla FROM sca.destino WHERE dstid={$dados['dstid']}) as destino
		    ";
		
		$rsLocal = $this->pegaLinha( $sql );
		
		// Verifica Foto
		$idArquivo = $this->recuperarIdFoto( $dados ['vttid'], null );

		if ($idArquivo) {
			$FileSimec = new FilesSimec ( "visitantefoto", array (), "sca" );
			
			$foto = $FileSimec->getArquivo ( $idArquivo );

			$chave = md5 ( 'etiqueta_comfoto' );
			$foto = $this->getImagem ( $chave, $foto );

			if ($foto) {
				$DADOS_FOTO = $foto;
			} else {
				$DADOS_FOTO = $imgFotoPadrao;
			}
		} else {
			$DADOS_FOTO = $imgFotoPadrao;
		}
		
		/*
		 * Atenção: Limite do tamanho do nome da pessoa sem quebrar o pdf, caso retirado o pdf não é gerado.
		 */
		$DADOS_NOME = trim ( $dados ['vttnome'] );
		$strNome = explode ( "[ ]+", $DADOS_NOME );
		$DADOS_NOME = $strNome [0] . "  " . $strNome [1];
		$DADOS_NOME = substr ( $DADOS_NOME, 0, 25 );
		
		$numeroCrachaSistema = trim ( substr ( $dados ['vsnumcrachasistema'], 4, 10 ) );
		$anoCracha = trim ( substr ( $dados ['vsnumcrachasistema'], 0, 4 ) );
		
		// Caso numeroCracha seja vazio, será preenchido com o valor informado no formulario
		if ($numeroCrachaSistema == "") {
			$numeroCrachaSistema = str_pad ( $dados ['vstnumcracha'], 6, "0", STR_PAD_LEFT );
		}
		
		// Caso anoCracha seja vazio, será preenchido com o ano recuperado do formulario
		if ($anoCracha == "") {
			$anoCracha = substr ( $dados ['vstdatentrada'], 0, 4 );
		}
		
		$DADOS_NUMERO = 'N:' . $numeroCrachaSistema . '/' . $anoCracha;
		
		$DADOS_DATA = date ( 'd/m/Y H:i' );
		$DADOS_LOCAL = trim ( $rsLocal ['edificio'] ) . ' ' . trim ( str_replace ( array ('º'), 'o', $rsLocal ['andar'] ) ) . ' ' . trim ( $rsLocal ['destino'] );
		$DADOS_LOCAL = substr ( $DADOS_LOCAL, 0, 26 );

		$arquivoHtml = " <html>
                    <head>
                    <style>
                        @page {
                            margin-top: 0;
                            margin-left: 0;
                            margin-bottom: 0;
                            margin-right: 0;
                        }
                    </style>
                    <title></title>
                    </head>
                    <body>
                    <table border='0' cellpadding='1'>
                    <tr>
                        <td>
                            <img width='85' height='95' src='data:image/jpg;base64," . $DADOS_FOTO . "'/>
                        </td>
                        <td>
                            <table width='100%'  border='0' cellpadding='1'>
                            <tr>
                                <td><img src='data:image/png;base64," . $imgBrasao . "'></td>
                            </tr>
                            <tr>
                                <td>
                                    <div style='line-height:9pt; font-family:monospace; font-size: 9pt'>$DADOS_DATA  $DADOS_NUMERO</div>
                                    <div style='padding-top: 2px; line-height:11pt; font-family:monospace; font-size: 11pt'>$DADOS_LOCAL</div>
                                    <div style='padding-top: 2px; line-height:11pt; font-family:monospace; font-size: 11pt'>$DADOS_NOME &nbsp;</div>
                                </td>
                            </tr>
                            </table>
                        </td>
                    </tr>
                    </table>
                    </body>
                    </html>";

		$nomeArquivo = 'MEC_ETIQUETA_' . $dados ['vttid'];
		$diretorio = APPRAIZ . 'arquivos/sca/etiquetas_tmp/';
		$diretorioArquivo = $diretorio . $nomeArquivo . '.pdf';
		

		if (!is_dir( $diretorio )) {
			mkdir ( $diretorio );
		}
		
		$dompdf = new DOMPDF ();
		$papel = array (0, 0, 249.45, 80.00);
		$dompdf->set_paper ( $papel, 'portrait' );
		$dompdf->load_html ( $arquivoHtml );
		$dompdf->render ();
		
		$pdfoutput = $dompdf->output ();
		$fp = fopen ( $diretorioArquivo, "w+" );
		
		stream_set_write_buffer ( $fp, 0 );
		fwrite ( $fp, $pdfoutput );
		fclose ( $fp );
		
		return $diretorioArquivo;
    }

	public function getImagem($key, $pathImg){
		$fileContent = file_get_contents($pathImg);
		$res = base64_encode($fileContent);
		return $res;
		$tempocache = 86400;
		try {
			if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch($key);}

			if ($cache_result) {
				$res = $cache_result;
			} else {
				if(file_exists( $pathImg )){
					$fileContent = file_get_contents($pathImg);
					$res = base64_encode($fileContent);
					
					if (function_exists('zend_shm_cache_store')) {
						if(zend_shm_cache_store($key, $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
					}
					
				}
			}
		} catch (Exception $e){
			if($_SESSION['usucpf'] = ''){
				echo $e->getMessage(); exit;
			} else {
				if(file_exists( $pathImg )){
					$fileContent = file_get_contents($pathImg);
				}
			}

		}
		return $res;
	}
}