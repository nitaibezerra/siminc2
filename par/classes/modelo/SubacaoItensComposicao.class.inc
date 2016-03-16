<?php

class SubacaoItensComposicao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaoitenscomposicao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "icoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'icoid' => null,
        'icoano' => null,
        'icoordem' => null,
        'icodescricao' => null,
        'icoquantidade' => null,
        'icoquantidadetecnico' => null,
        'icovalor' => null,
        'icovalortotal' => null,
        'icostatus' => null,
        'sbaid' => null,
        'unddid' => null,
        'icodetalhe' => null,
        'usucpf' => null,
        'dtatualizacao' => null,
        'picid' => null,
        'gicid' => null,
        'dicid' => null,
        'icovalidatecnico' => null,
        'icosequencia' => null
    );

    public function carregaItensComposicaoPorSbaid($sbaid, $anoref)
    {
        $sql = "SELECT
					unddid || '_' || icoid || '' as codigo,
					icoid,
				  	icoano,
				  	icoordem,
		    		icodescricao as descricao,
		    		icodescricao,
		    		icoquantidade,
				    icovalor,
				    icovalortotal,
				    icostatus,
				    sbaid,
				    unddid,
				    und.unddsc,
				    ics.icodetalhe
				FROM {$this->stNomeTabela} ics
				INNER JOIN par.unidademedida und ON und.undid = ics.unddid
				WHERE ics.sbaid = {$sbaid}
				AND icoano = '{$anoref}'
				ORDER BY icodescricao";

        return $this->carregar($sql);
    }

    public function recuperaDescricaoItem($iditem)
    {
        return $this->pegaUm("select icodescricao from {$this->stNomeTabela} where icoid = {$iditem}");
    }

    public function retornaItensComposicaoPorSubacao($sbaid,$icoano)
    {
        $sql = "select
					picid,
					icoquantidade
				from
					{$this->stNomeTabela}
				where
					sbaid = $sbaid
				and
					icoano = $icoano
				and
					icostatus = 'A'";
        $arrDados = $this->carregar($sql);
        if($arrDados){
            foreach($arrDados as $dado){
                $arrQtde[$dado['picid']] = $dado['icoquantidade'];
            }
            return $arrQtde;
        }else{
            return false;
        }
    }

    public function recuperaItemPorSubacao($sbaid,$icoano,$picid,$dicid=null,$icosequencia=null)
    {
        $str = "";
        if( $dicid ){
            $str = " AND dicid = ".$dicid;
        }
        if( $icosequencia ){
            $str .= " AND icosequencia = ".$icosequencia;
        }
        $sql = "select
					icoid
				from
					{$this->stNomeTabela}
				where
					sbaid = $sbaid
				and
					icoano = $icoano
				and
					picid = $picid
				and
					icostatus = 'A'".$str;
        return $this->pegaUm($sql);
    }

    function montaLista($sbaid,$icoano = null, $sbacronograma = null, $valida = 'true', $reformulacao = 'false', $finalizado = false){
        $v = "";
        $grup = "";
        $totalizador = "";
        $mostra = false;
        $inuid = '';

        if ($_SESSION['par']['inuid']) {
            $inuid = $_SESSION['par']['inuid'];
        }else if ($_REQUEST['inuid']) {
            $inuid = $_REQUEST['inuid'];
        }else {

            $sql = "SELECT iu.inuid FROM par.subacao s
					INNER JOIN par.acao a ON a.aciid = s.aciid
					INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
					INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
					WHERE s.sbaid IN (".$sbaid.")";

            $inuid = $this->pegaUm( $sql );
        }
        $docid = parPegarDocidParaEntidade($inuid);

        $sql = "SELECT iu.itrid FROM par.subacao s
				INNER JOIN par.acao a ON a.aciid = s.aciid
				INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
				INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
				WHERE s.sbaid IN (".$sbaid.")";

        $itrid = $this->pegaUm( $sql );
        $sqlInu = "SELECT * FROM par.instrumentounidade WHERE inuid = $inuid";
        $muncodInu = $this->pegaLinha($sqlInu);
        if( $itrid == '1' ){
            $tcpid = 1;
        }else{
            $tcpid = 3;
            if (trim($muncodInu['muncod']) == '5300108' || $muncodInu['estuf'] == 'DF') {
                $tcpid = 1;
            }
        }

        $estadoAtual = wf_pegarEstadoAtual( $docid );
        $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

        $sql = "select docid from par.subacao where sbaid = ".$sbaid;
        $docid = $this->pegaUm($sql);
        $estadoAtualSubacao = wf_pegarEstadoAtual( $docid );

        if( (	in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ||
                in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) ||
                in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) ||
                in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil)
            )
            &&  ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE  || $reformulacao == 'true' ) ){
            $habilItens = "";
            $habilItensTecnico = "";
        }else{
            $habilItens = "disabled=disabled";
            $habilItensTecnico = "disabled=disabled";
        }

        $habilTemp = 'disabled=disabled';

        /*
         * REGRA QUE LIBERA EDIÇÃO PARA OS USUÁRIOS
         * RENILDA PERES DE LIMA
         * JULIO CEZAR DA CAMARA RIBEIRO VIANA
         * CRIADA EM 07/08/2012 POR WESCLEY
         * DEMANDANTE: THIAGO TASCA
         */
        if( in_array($_SESSION['usucpf'], array('', '')) ){
            $habilItens = "";
            $habilTemp = "";
        }
        /* fim */

        if( ( 	in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ||
                in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) ||
                in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) ||
                in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil)
            ) &&  ($estadoAtualSubacao["esdid"] != WF_SUBACAO_ELABORACAO ) ){
            $habilItens = "";
            $habilItensTecnico = "";
            $mostra = true;
        }


        $exceptionVisuMun = false;

        if( ( 	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
            in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
            in_array(PAR_PERFIL_PREFEITO,$arrayPerfil)
        ) ){

            if($reformulacao == 'true')
            {
                $mostraME = true;
                $habilItens = "";
                $habilItensTecnico = "disabled=disabled";
            }
            else if(($estadoAtualSubacao["esdid"] != WF_SUBACAO_ELABORACAO ))
            {
                $mostraME = true;
                if(($estadoAtualSubacao["esdid"] != WF_SUBACAO_ANALISE ) )
                {
                    $exceptionVisuMun = true;
                }
            }

        }


        if( in_array(PAR_PERFIL_PREFEITO,$arrayPerfil) && $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA){
            $habilItens = "";
            $habilItensTecnico = 'disabled=\"disabled\"';
            $mostraME = true;
        }

        if( $finalizado == true ){
            $habilItens = "";
            $habilItensTecnico = 'disabled=\"disabled\"';
            $mostraME = true;
        }

        if( $icoano == 'totalizadores' ){
            if( $sbacronograma == 1 ){
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) || $mostra == true ){
                    $totalizador = "icoquantidade as icoqtd,
									icoquantidadetecnico as icoqtdtecnico,
									icovalor as valor,
									TO_CHAR(icoquantidade*icovalor, '000G000G000D99') as total,
									CASE WHEN icovalidatecnico = 'S' THEN 'Validado' ELSE '' END as itemaprovado ";
                } else {
                    $totalizador = "icoquantidade as icoqtd,
									icovalor as valor,
									TO_CHAR(icoquantidade*icovalor, '000G000G000D99') as total ";
                }

            } elseif( $sbacronograma == 2 ) {
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) || $mostra == true  ){
                    $totalizador = "sum(seiqtd) as icoqtd,
									sum(seiqtdtecnico) as icoqtdtecnico,
									sic.icovalor as valor,
									TO_CHAR(sum(seiqtd*sic.icovalor), '000G000G000D99') as total,
									CASE WHEN icovalidatecnico = 'S' THEN 'Validado' ELSE '' END as itemaprovado ";
                } else {
                    $totalizador = "sum(seiqtd) as icoqtd,
									sic.icovalor as valor,
									TO_CHAR(sum(seiqtd*sic.icovalor), '000G000G000D99') as total ";
                }
            }
            if( $reformulacao == 'true' ){
                $grup = ", icoano, sic.icoid, sic.icovalidatecnico";
            } else {
                $grup = ", sic.icoid, sic.icovalidatecnico";
            }
        }elseif( $icoano ){
            if( $sbacronograma == 1 ){
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true || $reformulacao == 'true' ){
                    $grup = ", icoano, sic.icoid, icovalidatecnico, icoquantidadetecnico ";
                } else {
                    $grup = ", icoano, sic.icoid, icoquantidadetecnico ";
                }
            } elseif( $sbacronograma == 2) {
                $v = "sic.";
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true || $reformulacao == 'true' ){
                    $grup = ", sic.icoano, sic.icoid, icovalidatecnico";
                } else {
                    $grup = ", sic.icoano, sic.icoid, icovalidatecnico";
                }
            } else {
                $grup = ", sic.icoano, sic.icoid";
            }

            $ano = "AND	{$v}icoano = $icoano";
        }

        if( $sbacronograma == 1 ){
            $sql = "SELECT DISTINCT ";
            if($valida == 'true' && $icoano != 'totalizadores' && ($estadoAtualSubacao["esdid"] != WF_SUBACAO_ANALISE) || $reformulacao == 'true'){
                $sql .= "CASE WHEN (select icoid from par.subacaoitenscomposicaocontratos where icoid = sic.icoid limit 1) IS NULL
								THEN
										CASE WHEN EXISTS (
											select prgid, * from par.subacaoitenscomposicao sicc
											INNER JOIN par.subacao sbaa on sbaa.sbaid = sicc.sbaid
											where sbaa.sbastatus = 'A'
											and sbaa.prgid = 153
											and sicc.icoid = sic.icoid
											)
										THEN
											'<input type=\"hidden\" value\"'|| sic.icoid || '\">'
										ELSE
											'<center><img class=\"middle link\" title=\"Excluir Item\" onclick=\"deletarItem(this, '|| sic.icoid ||', '|| sic.icoano ||' )\"
												   src=\"../imagens/excluir.gif\" /></center>'
										END

                                                                WHEN sic.icoquantidaderecebida IS NULL THEN
                                                                    '<center><img class=\"middle link\" title=\"Item não pode ser excluido pois se encontra em acompanhamento.\"
                                                                        onclick=\"alert(''Item não pode ser excluido pois se encontra em acompanhamento. Este item possui contrato vinculado.'')\"
                                                                        src=\"../imagens/restricao.png\" />
                                                                    </center>'
								ELSE '<center>
										<img class=\"middle link\" title=\"Item não pode ser excluido pois se encontra em acompanhamento.\"
											 onclick=\"alert(''Item não pode ser excluido pois se encontra em acompanhamento. Este item deve obedecer uma quantidade minima de '||sic.icoquantidaderecebida||', sendo esta a quantidade já monitorada.'')\"
											 src=\"../imagens/restricao.png\" />
										<input id=\"vlrmin_'||sic.icoid||'\" type=\"hidden\"value=\"'||sic.icoquantidaderecebida||'\">
										<input name=\"icoid_monitorado\" type=\"hidden\" value=\"'||sic.icoid||'\">
									  </center>'
							END as acao,";
                $grup .= ", sic.icoquantidaderecebida";
// 					$sql .= "'<center><img class=\"middle link\" title=\"Excluir Item\" onclick=\"deletarItem(this, '|| sic.icoid ||', '|| sic.icoano ||' )\"  src=\"../imagens/excluir.gif\" /></center>' as acao,";
            }else{
                $sql .= "'<center><img class=\"middle link\" title=\"Excluir Item\" src=\"../imagens/excluir_01.gif\" /></center>' as acao,";
            }
            $sql .= "sic.icodescricao, ";
            if( $totalizador ){
                $sql .= $totalizador;
            } else {
                $sql .= "'<center>' || sic.icoquantidade || '</center>' as qtd,";
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true ){
                    $sql .= "'<input size=\"10\" id=\"icoqtdtecnico' || sic.icoid || '\"
								type=\"text\"
								class=\"CampoEstilo\"
								onmouseover=\"MouseOver(this);\"
								onfocus=\"MouseClick(this);\"
								onmouseout=\"MouseOut(this);\"
								onblur=\"MouseBlur(this); calcularValorTotal(' || sic.icoid || ');\"
								onfocus=\"this.select()\"
								name=\"icoqtdtecnico[{$icoano}]['|| sic.icoid ||']\"
								{$habilItensTecnico}
								' || CASE WHEN sic.icovalidatecnico = 'S' THEN '' ELSE 'disabled=disabled' END || '
								value=\"' ||
									case
							    		when substring( trim(to_char(coalesce(sic.icoquantidadetecnico, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sic.icoquantidadetecnico, 0), '9999999990D99'))) +1, 2 ) = '00'
							    			then trim(to_char(coalesce(sic.icoquantidadetecnico, 0), '999999999'))
							    		else trim(to_char(coalesce(sic.icoquantidadetecnico, 0), '9999999990D99'))
							    	END
								|| '\" />' as icoqtdtecnico,";
                }
                $sql .= "TO_CHAR(sic.icovalor, '000G000G000D99') as valor,
							TO_CHAR((sic.icoquantidade*icovalor), '000G000G000D99') as totalplanejado,
						--	TO_CHAR((sic.icoquantidadetecnico*icovalor), '000G000G000D99') as total

							CASE WHEN icovalidatecnico IS NOT NULL THEN
								CASE WHEN icovalidatecnico = 'S'
								THEN
									TO_CHAR((sic.icoquantidadetecnico*icovalor), '000G000G000D99')
								END
							END as total

							";
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true ){
                    $sql .= ",'<center>' ||";
                    if(empty($habilItensTecnico)){
                        $sql .= "
                                                    '<input type=\"radio\" name=\"valida[{$icoano}][' || sic.icoid || ']\"
                                                        id=\"valida[{$icoano}][' || sic.icoid || ']\" value=\"S\" ' || CASE WHEN sic.icovalidatecnico = 'S' THEN 'checked' ELSE '' END || '
                                                        onclick=\"validaItem(this,$sbaid,$icoano,' || sic.icoid || ',\'global\')\">Sim
                                                    <input type=\"radio\" name=\"valida[{$icoano}][' || sic.icoid || ']\"
                                                        id=\"valida[{$icoano}][' || sic.icoid || ']\" value=\"N\" ' || CASE WHEN sic.icovalidatecnico = 'N' THEN 'checked' ELSE '' END || '
                                                        onclick=\"validaItem(this,$sbaid,$icoano,' || sic.icoid || ',\'global\')\">Não' ";
                    } else {
                        $sql .= "
                                                    CASE WHEN sic.icovalidatecnico = 'S' THEN
                                                        'Sim'
                                                    WHEN sic.icovalidatecnico = 'N' THEN
                                                        'Não'
                                                    ELSE
                                                        ''
                                                    END
                                                ";
                    }
                    $sql .= " || '</center>' AS itemaprovado ";
                }
            }
            /*GROUP BY sic.icodescricao, sic.icovalor, sic.icoid, sic.icovalidatecnico, icc.icoid {$grup}
             * */
            $sql .= " FROM
						par.subacaoitenscomposicao sic
					LEFT JOIN par.subacaoitenscomposicaocontratos 	icc ON icc.icoid = sic.icoid
					WHERE
						sic.icostatus = 'A'
					AND
						sic.sbaid = $sbaid
					{$ano}
					ORDER BY
						sic.icodescricao";
        } else {
            $sql = "SELECT DISTINCT ";
            if($valida == 'true' && $icoano != 'totalizadores'  && ($estadoAtualSubacao["esdid"] != WF_SUBACAO_ANALISE) || $reformulacao == 'true'){
                $sql .= "CASE WHEN (select icoid from par.subacaoitenscomposicaocontratos where icoid = sic.icoid limit 1) IS NULL
								THEN
									CASE WHEN EXISTS (
											select prgid, * from par.subacaoitenscomposicao sicc
											INNER JOIN par.subacao sbaa on sbaa.sbaid = sicc.sbaid
											where sbaa.sbastatus = 'A'
											and sbaa.prgid = 153
											and sicc.icoid = sic.icoid
											)
										THEN '<input type=\"hidden\" value\"'|| sic.icoid || '\">'
										ELSE '<center><img class=\"middle link\" title=\"Excluir Item\" onclick=\"deletarItem(this, '|| sic.icoid ||', '|| sic.icoano ||' )\" src=\"../imagens/excluir.gif\" /></center>'
									END
								WHEN sic.icoquantidaderecebida IS NULL
								THEN '<center><img class=\"middle link\" title=\"Item não pode ser excluido pois se encontra em acompanhamento.\"
													onclick=\"alert(''Item não pode ser excluido pois se encontra em acompanhamento. Este item possui contrato vinculado.'')\"
													src=\"../imagens/restricao.png\" />
									</center>'
								ELSE '<center>
											<img class=\"middle link\" title=\"Item não pode ser excluido pois se encontra em acompanhamento.\"
												onclick=\"alert(''Item não pode ser excluido pois se encontra em acompanhamento. Este item deve obedecer uma quantidade minima de '||
																	sic.icoquantidaderecebida||', sendo esta a quantidade já monitorada.'')\"
												src=\"../imagens/restricao.png\" />
											<input id=\"vlrmin_'||sic.icoid||'\" type=\"hidden\"value=\"'||sic.icoquantidaderecebida||'\">
										</center>'
							END as acao,";
                $grup .= ", sic.icoquantidaderecebida";
            }else{
                $sql .= "'<center><img class=\"middle link\" title=\"Excluir Item\" src=\"../imagens/excluir_01.gif\" /></center>' as acao,";
            }
            $sql .= "sic.icodescricao,";
            if( $totalizador ){
                $sql .= $totalizador;
            } else {
                $sql .= "
						'<input size=\"10\" id=\"icoqtd' || sic.icoid || '\"
								type=\"text\"
								class=\"CampoEstilo\"
								onmouseover=\"MouseOver(this);\"
								onfocus=\"MouseClick(this);\"
								onmouseout=\"MouseOut(this);\"
								onblur=\"MouseBlur(this); calcularValorTotal(' || sic.icoid || ');\"
								onfocus=\"this.select()\"
								onclick=\"return popupSelecionarEscolasItemComposicao($sbaid,$icoano,' || sic.icoid || ',\'esc\')\"
								name=\"icoqtd[{$icoano}]['|| sic.icoid ||']\"
								readonly=\"readonly\"";
                if( (($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostraME == true || $mostra == true) && ($exceptionVisuMun != true)  ){
                    $sql .= "' || CASE WHEN icovalidatecnico = 'S' THEN '{$habilItens}' ELSE '' END || '";
                }
                $sql .= " value=\"' ||
									case
							    		when substring( trim(to_char(coalesce(sum(seiqtd), 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sum(seiqtd), 0), '9999999990D99'))) +1, 2 ) = '00'
							    			then trim(to_char(coalesce(sum(seiqtd), 0), '999999999'))
							    		else trim(to_char(coalesce(sum(seiqtd), 0), '9999999990D99'))
							    	END
								|| '\" />' as icoqtd,";
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true  ){


                    if( ( 	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
                            in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
                            in_array(PAR_PERFIL_PREFEITO,$arrayPerfil)
                        ) && $mostraME == true )
                    {
                        $sql .= "'<img width=\'18px\' height=\'18px\' src=../imagens/icones-escola.png title=abrir style=cursor:pointer;

									onclick=\"return popupSelecionarEscolasItemComposicao($sbaid,$icoano,' || sic.icoid || ',\'visu\')\"
									>

									' ||
									case
									when substring( trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99'))) +1, 2 ) = '00'
									then trim(to_char(coalesce(sum(seiqtdtecnico), 0), '999999999'))
									else trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99'))
									END
									as icoqtdtecnico,";
                    }
                    else
                    {

                        $sql .= "'<input size=\"10\" id=\"icoqtdtecnico' || sic.icoid || '\"
									type=\"text\"
									class=\"CampoEstilo\"
									onmouseover=\"MouseOver(this);\"
									onfocus=\"MouseClick(this);\"
									onmouseout=\"MouseOut(this);\"
									onblur=\"MouseBlur(this); calcularValorTotal(' || sic.icoid || ');\"
									onfocus=\"this.select()\"
									onclick=\"return popupSelecionarEscolasItemComposicao($sbaid,$icoano,' || sic.icoid || ',\'tec\')\"
									name=\"icoqtdtecnico[{$icoano}]['|| sic.icoid ||']\"
									readonly=\"readonly\"
									{$habilItensTecnico}
									' || CASE WHEN icovalidatecnico = '' THEN 'disabled=disabled' ELSE '' END || '
									value=\"' ||
										case
								    		when substring( trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99'))) +1, 2 ) = '00'
								    			then trim(to_char(coalesce(sum(seiqtdtecnico), 0), '999999999'))
								    		else trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99'))
								    	END
									|| '\" />' as icoqtdtecnico,";
                    }
                }
                elseif( ( 	in_array(PAR_PERFIL_EQUIPE_ESTADUAL,$arrayPerfil) ||
                        in_array(PAR_PERFIL_EQUIPE_MUNICIPAL,$arrayPerfil) ||
                        in_array(PAR_PERFIL_PREFEITO,$arrayPerfil)
                    ) && $mostraME == true )
                {
                    $sql .= "'<img width=\'18px\' height=\'18px\' src=../imagens/icones-escola.png title=abrir style=cursor:pointer;

							onclick=\"return popupSelecionarEscolasItemComposicao($sbaid,$icoano,' || sic.icoid || ',\'visu\')\"
							>

							' ||
							case
							when substring( trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99'))) +1, 2 ) = '00'
							then trim(to_char(coalesce(sum(seiqtdtecnico), 0), '999999999'))
							else trim(to_char(coalesce(sum(seiqtdtecnico), 0), '9999999990D99'))
							END
							 as icoqtdtecnico,";
                }
                $sql .= "TO_CHAR(sic.icovalor, '000G000G000D99') as valor,

						TO_CHAR(sum((seiqtd*sic.icovalor)), '000G000G000D99') as totalplanejado,

						CASE WHEN icovalidatecnico IS NOT NULL THEN
							CASE WHEN icovalidatecnico = 'S'
							THEN
								TO_CHAR(sum((seiqtdtecnico*sic.icovalor)), '000G000G000D99')
							END
						ELSE
							TO_CHAR(sum((seiqtd*sic.icovalor)), '000G000G000D99')
						END as total";
                if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true  || $mostraME == true ){
                    $sql .= ",'<center>' || ";
                    if(empty($habilItensTecnico)){
                        $sql .= "
                                                            '<input type=\"radio\" name=\"valida[{$icoano}][' || sic.icoid || ']\"
                                                                id=\"valida[{$icoano}][' || sic.icoid || ']\" value=\"S\" ' || CASE WHEN icovalidatecnico = 'S' THEN 'checked' ELSE '' END || '
                                                                onclick=\"validaItem(this,$sbaid,$icoano,' || sic.icoid || ',\'esc\')\">Sim
                                                            <input type=\"radio\" name=\"valida[{$icoano}][' || sic.icoid || ']\"
                                                                id=\"valida[{$icoano}][' || sic.icoid || ']\" value=\"N\" ' || CASE WHEN icovalidatecnico = 'N' THEN 'checked' ELSE '' END || '
                                                                onclick=\"validaItem(this, $sbaid,$icoano,' || sic.icoid || ',\'esc\')\">Não' ";
                    } else {
                        $sql .= "
                                                            CASE WHEN icovalidatecnico = 'S' THEN
                                                                'Sim'
                                                            WHEN icovalidatecnico = 'N' THEN
                                                                'Não'
                                                            ELSE
                                                                NULL
                                                            END
                                                        ";
                    }
                    $sql .= " || '</center>' AS itemaprovado";
                }
            }
            $sql .=	"
				FROM
					par.subacaoitenscomposicao sic
				LEFT JOIN
					par.subacaoescolas ses
						INNER JOIN par.escolas esc ON esc.escid = ses.escid
						INNER JOIN entidade.entidade t ON t.entid = esc.entid and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and t.tpcid = $tcpid
						inner join entidade.funcaoentidade f on f.entid = t.entid AND f.funid = 3
					on ses.sbaid = sic.sbaid AND ses.sesstatus = 'A'
				LEFT JOIN
					par.subescolas_subitenscomposicao ssc ON ssc.icoid = sic.icoid AND ssc.sesid = ses.sesid
				WHERE
					sic.icostatus = 'A'
					AND sic.sbaid = $sbaid
				{$ano}
				GROUP BY
					sic.icodescricao, sic.icovalor, sic.icoid, icovalidatecnico $grup
				ORDER BY
					sic.icodescricao";
        }

        if( $valida == 'true'  && $icoano != 'totalizadores'){
            if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA  || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO) || $mostra == true ){
                $cabecalho = array("Ação", "Item","Qtde","Qtde Técnico","Valor Unitário","Valor Planejado","Valor Aprovado","Item Aprovado");
                $soma = array(4 => 'S',5 => 'S',6 => 'S');
            } else {
                $cabecalho = array("Ação", "Item","Qtde","Valor Unitário","Valor Planejado","Valor Aprovado");
                $soma = array(3 => 'S',4 => 'S',5 => 'S');
            }
        } else {
            if( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA  || $estadoAtualSubacao["esdid"] == WF_SUBACAO_EMPENHO)  || $mostraME == true || $mostra == true ){
                $cabecalho = array("Ação", "Item","Qtde","Qtde Técnico","Valor Unitário","Valor Planejado","Valor Aprovado","Item Aprovado");
                $soma = array(3 => 'S',4 => 'S',5 => 'S', 6 => 'S');
            } else {
                $cabecalho = array("Ação", "Item","Qtde","Valor Unitário","Valor Planejado","Valor Aprovado");
                $soma = array(2 => 'S',3 => 'S',4 => 'S');
            }
        }
        return $this->monta_lista_simples($sql,$cabecalho,501,5,$soma,"100%","S");
    }

    function conta($sbaid,$icoano = null)
    {
        $ano = "AND	icoano = $icoano";
        $sql = "SELECT
					count(icodescricao)
				FROM
					par.subacaoitenscomposicao sic
				WHERE
					sic.icostatus = 'A'
				AND
					sic.sbaid = $sbaid
				{$ano}";

        return $this->pegaUm($sql);
    }

    public function deletaPorPtoid($ptoid)
    {
        $sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid IN (SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid})));";

        $sql.= "DELETE FROM par.subacaoitenscomposicao WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}));";

        $this->executar( $sql );
    }

    public function deletaPorSbaid($sbaid, $ano = array())
    {
        $anos = implode( $ano, "," );
        $sql = "DELETE FROM par.subescolas_subitenscomposicao WHERE icoid IN (SELECT icoid FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND icoano IN ({$anos}));";

        $sql.= "DELETE FROM par.subacaoitenscomposicao WHERE sbaid = {$sbaid} AND icoano IN ({$anos});";

        $this->executar( $sql );
    }

    public function listarSubacaoItensComposicaoPorSbdid($arrParam, $booSQL = FALSE) {

        $sql = "SELECT  DISTINCT
				si.icodescricao,
				( CASE
					WHEN sbacronograma = 1 THEN ( 	SELECT DISTINCT
														CASE WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(sic.icoquantidadetecnico,0))  END as vlrsubacao
													FROM
														par.subacaoitenscomposicao sic
													WHERE
														sic.sbaid = s.sbaid
														AND sic.icoano = sd.sbdano
														and sic.icoid = si.icoid
                                                                                                                AND sic.icostatus = 'A'
													GROUP BY sic.sbaid, sic.icovalidatecnico )
					ELSE (	SELECT DISTINCT
								CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN sum(coalesce(se.sesquantidadetecnico,0))-- escolas sem itens
								ELSE CASE
										WHEN sic.icovalidatecnico = 'S' THEN sum(coalesce(ssi.seiqtdtecnico,0)) -- validado (caso não o item não é contado) -- escolas com itens
									END
								END as vlrsubacao
							FROM
								entidade.entidade t
							INNER JOIN entidade.funcaoentidade 				f   ON f.entid = t.entid
							LEFT  JOIN entidade.entidadedetalhe 			ed  ON t.entid = ed.entid
							INNER JOIN entidade.endereco 					d   ON t.entid = d.entid
							LEFT  JOIN territorios.municipio 				m   ON m.muncod = d.muncod
							LEFT  JOIN par.escolas 							e   ON e.entid = t.entid
							INNER JOIN par.subacaoescolas 					se  ON se.escid = e.escid
							INNER JOIN par.subacaoitenscomposicao 			sic ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
							LEFT  JOIN par.subescolas_subitenscomposicao 	ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
							WHERE
								sic.sbaid = s.sbaid
								AND sic.icoano = sd.sbdano
								AND sic.icoid = si.icoid
								AND (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 --and t.tpcid = v_tpcid
                        		AND ( CASE WHEN inu.itrid = 2 THEN t.tpcid = 3 ELSE t.tpcid = 1 END ) --Filtra por escolar da esfera da subação.
							GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico )
				END ) as quantidade,
				'<div style=\"width:48%; float:left; text-align: center\">' || si.icoquantidaderecebida || '</div><div style=\"width:48%; float:left; text-align: center\"><img src=\"../imagens/schedule.png\" title=\"Histórico de Acompanhamento: Monitoramento\" style=\"cursor:pointer\" onclick=\"historicoAcompanhamento('||si.icoid||', \'monitoramento\')\"></div>' as qtd_recebida,
				'<center>' || (
                   array_to_string(array(
                        SELECT
                            CASE WHEN (con.condiligencia) IS NOT NULL 
                                THEN '<a href=\"javascript:void();\" onclick=\"visualizarDocumentoPdf('|| con.arqid||', \'Contrato\', '||con.connumerocontrato||', '||con.conid||', {$arrParam['dopid']}, '||sic.icoid||' )\" ><span style=\"color: #FF0000\" title=\"Contrato em Diligência\">' || con.connumerocontrato || '</span></a>'
                                ELSE '<a href=\"javascript:void();\" onclick=\"visualizarDocumentoPdf('|| con.arqid||', \'Contrato\', '||con.connumerocontrato||', '||con.conid||', {$arrParam['dopid']}, '||sic.icoid||' )\" ><span>' || con.connumerocontrato || '</span></a>'
                            END
                        FROM par.subacaoitenscomposicaocontratos sicc
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicc.icoid AND sic.icostatus = 'A'
                        INNER JOIN par.contratos con ON con.conid=sicc.conid AND con.constatus = 'A'
                        WHERE sic.sbaid = s.sbaid AND sic.icoid = si.icoid AND sicc.sccstatus = 'A'), '<br>')
                ) || '</center>' as contratos_item,
				'<center>' || (
				    array_to_string(array(
                        SELECT
                            CASE WHEN (nf.ntfdiligencia) IS NOT NULL 
                                THEN '<a href=\"javascript:void();\" onclick=\"visualizarDocumentoPdf('|| nf.arqid||', \'Nota Fiscal\', '||nf.ntfnumeronotafiscal||', '||nf.ntfid||', {$arrParam['dopid']}, '||sic.icoid||')\" ><span style=\"color: #FF0000\" title=\"Nota Fiscal em Diligência\">' || nf.ntfnumeronotafiscal || '</span></a>'
                                ELSE '<a href=\"javascript:void();\" onclick=\"visualizarDocumentoPdf('|| nf.arqid||', \'Nota Fiscal\', '||nf.ntfnumeronotafiscal||', '||nf.ntfid||', {$arrParam['dopid']}, '||sic.icoid||')\" ><span>' || nf.ntfnumeronotafiscal || '</span></a>'
                            END ||
                            CASE WHEN (nf.ntfvalidado) IS TRUE THEN ' <input type=\"checkbox\" ntfid=\"'||nf.ntfid||'\" class=\"notas_{$arrParam['dopid']}\" value=\"'||nf.ntfid||'\" checked=\"checked\" disabled=\"disabled\" />' ELSE ' <input type=\"checkbox\" ntfid=\"'||nf.ntfid||'\" class=\"notas_{$arrParam['dopid']}\" value=\"'||nf.ntfid||'\" />' END
                        FROM par.subacaoitenscomposicaonotasfiscais sicnf
                        INNER JOIN par.subacaoitenscomposicao sic ON sic.icoid=sicnf.icoid AND sic.icostatus = 'A'
                        INNER JOIN par.notasfiscais nf ON nf.ntfid=sicnf.ntfid AND nf.ntfstatus = 'A'
                        WHERE sic.sbaid = s.sbaid AND sic.icoid = si.icoid AND sicnf.scnstatus = 'A'), '<br>')
                ) || '</center>' as notas_item,
                       CASE WHEN (SELECT 
                                    SUM(foo.qtd) as qtd
                                FROM (
                                    SELECT 
                                            COALESCE(COUNT(con.conid), 0) AS qtd
                                    FROM par.subacaoitenscomposicaocontratos scon 
                                    INNER JOIN par.contratos con ON con.conid = scon.conid
                                    WHERE scon.icoid = si.icoid AND con.condiligencia IS NOT NULL AND scon.sccstatus = 'A' 

                                    UNION ALL

                                    SELECT 
                                            COALESCE(COUNT(ntf.ntfid), 0) AS qtd
                                    FROM par.subacaoitenscomposicaonotasfiscais sntf 
                                    INNER JOIN par.notasfiscais ntf ON ntf.ntfid = sntf.ntfid
                                    WHERE sntf.icoid = si.icoid AND ntf.ntfdiligencia IS NOT NULL AND sntf.scnstatus = 'A'
                                ) AS foo) > 0 
                                THEN '<div style=\"text-align: center\"><img src=\"../imagens/schedule.png\" title=\"Histórico de Diligência\" style=\"cursor:pointer\" onclick=\"historicoDiligencia('||si.icoid||', \'monitoramento\')\"></div>' 
                                ELSE '' 
                            END as historico_diligencia
			FROM par.subacao s
			INNER JOIN par.acao aca on aca.aciid = s.aciid AND acistatus = 'A'
			INNER JOIN par.pontuacao pon on pon.ptoid = aca.ptoid AND ptostatus = 'A'
			INNER JOIN par.instrumentounidade inu on inu.inuid = pon.inuid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
			INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
			INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
			WHERE sd.sbdid = {$arrParam['sbdid']}
			ORDER BY si.icodescricao";
//          ver(simec_htmlentities($sql), d);
        return ($booSQL) ? $sql : $this->carregar($sql);
    }
}