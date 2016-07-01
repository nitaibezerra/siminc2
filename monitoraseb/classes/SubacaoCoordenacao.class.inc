<?php

include_once APPRAIZ . "includes/workflow.php";

class SubacaoCoordenacao extends Modelo{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitoraseb.subacaocoordenacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "scoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
                                          'catid' => null,
                                          'coonid' => null,
                                          'sbaid' => null
                                      );

    protected $arrPesquisa = null;

    public function excluiVinculo($vinId){
        return "";
    }

    public function carregaVinculos($coonid){
        $sql = "SELECT vinculo.scoid as scoid, vinculo.coonid as coonid, vinculo.sbaid as sbaid, suba.sbatitulo as sbatitulo, usuario.usunome as usunome FROM  monitoraseb.subacaocoordenacao vinculo
                JOIN monitora.pi_subacao suba ON (suba.sbaid = vinculo.sbaid)
                JOIN seguranca.usuario usuario ON (usuario.usucpf = vinculo.usucpf)
                WHERE vinculo.coonid = ".$coonid." order by suba.sbatitulo";
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

    public function carregaCoordenacoes(){
        $sisid = $_SESSION['sisid'];
        $usucpf = $_SESSION['usucpf'];
        $sql = "SELECT distinct coo.coonid as coonid, coo.coodsc as coodsc, coo.coosigla as coosigla, usuario.usunome as usunome FROM monitoraseb.coordenacao coo
                JOIN monitoraseb.usuarioresponsabilidade usuarioResp ON (usuarioResp.coonid = coo.coonid)
                JOIN seguranca.usuario usuario ON (usuario.usucpf = usuarioResp.usucpf)
                JOIN seguranca.perfilusuario pUsuario ON (pUsuario.usucpf = usuario.usucpf)
                JOIN seguranca.perfil perfil ON (pUsuario.pflcod = perfil.pflcod)
                WHERE usuario.usucpf ='$usucpf' and perfil.sisid = $sisid and ((usuarioResp.rpustatus = 'A') or (perfil.pflnivel = 1))
                order by coo.coodsc";
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

    public function montaListaSubacao($coonid){
        $listaCoordenadores = $this->recuperaListaCoordenadorTecnico($coonid);

        $html_tablela_listagem = "";

        $sql = "select s.sbaid, s.sbacod, s.sbatitulo
                  from monitora.pi_subacao s
                 where s.sbaid not in (select sc.sbaid
                                         from monitoraseb.subacaocoordenacao sc
                                        where sc.coonid != $coonid)
                 order by sbacod, sbatitulo";

        $listagem = $this->carregar($sql);
         $count = count($listagem);
         $html_tablela_listagem .= "
             <table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem' id='tabela'>
                 <tr>
                     <td align='center' bgcolor='#dcdcdc' colspan='4'>
                         <label style='color: rgb(0, 0, 0);' class='TituloTela'>Associar Subação à Coordenação</label>
                     </td>
                 </tr>
                 <tr bgcolor='#c5c5c5'>
                    <td align='center' width='20px'></td>
                    <td align='center' width='30px'><b>Código</b></td>
                    <td align='center' width='364px'><b>Nome</b></td>
                    <td align='center' width='400px'><b>Coordenador Técnico</b></td>
                </tr>
         ";
         if($count>0){
             for ($i = 0; $i < $count; $i++){
                $id    = $listagem[$i]["sbaid"];
                $codigo    = $listagem[$i]["sbacod"];
                $descricao = $listagem[$i]["sbatitulo"];
                $cor = ($i%2 == 0) ? '#f4f4f4' : '#e0e0e0';
                $selecionado = $this->verificaVinculo($coonid,$id);

                $combo = $this->montaComboCoordenadorTecnico($id, $listaCoordenadores, $selecionado);
                $checked = ($selecionado=="")?"":"checked='checked'";


                $html_tablela_listagem .= "
                    <tr bgcolor='".$cor."'>
                        <td>
                            <input type='checkbox' ".$checked." name='arraySubAcao[]' value='".$id."' onclick='habilitaCombo(this)' \\>
                        </td>
                        <td>
                            ".$codigo."
                        </td>
                        <td>
                            ".$descricao."
                        </td>
                        <td>
                            ".$combo."
                        </td>
                    </tr>";

            }
         }
         $html_tablela_listagem .= "</table>";

         return $html_tablela_listagem;

    }

    private function montaComboCoordenadorTecnico($codigo, $listagem, $selecionado){

        $combo = "";
         $count = count($listagem);
         $hablitado = ($selecionado=="")?"class='disabled' disabled":"class='CampoEstilo obrigatorio'";

         if($count>0){
             $combo .="<select style='width: 200px;' ".$hablitado." name='arrayCombos[]' id='combo_".$codigo."'>
                        <option value=''>Selecione...</option>";
             for ($i = 0; $i < $count; $i++){
                 $nome   = $listagem[$i]["usunome"];
                $usucpf = $listagem[$i]["usucpf"];
                $selecao = ($selecionado==$usucpf)?"selected":"";
                $combo .="<option value='".$codigo.":".$usucpf."' ".$selecao.">".$nome."</option>";
             }
            $combo .="</select>";
         }
         return $combo;
    }

    private function recuperaListaCoordenadorTecnico($coonid){
        $sql = "SELECT usuario.usunome as usunome,usuario.usucpf  as usucpf FROM seguranca.usuario usuario
            JOIN monitoraseb.usuarioresponsabilidade usuarioResp ON (usuario.usucpf = usuarioResp.usucpf)
            JOIN seguranca.perfil perfil ON (usuarioResp.pflcod = perfil.pflcod)
            WHERE perfil.pflnivel = 3 AND usuarioResp.rpustatus='A' AND usuarioResp.coonid = ".$coonid;

        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

    public function salvar($coonid,$arraySubAcao,$arrayCombos){

        $subacoesSelecionadas = "";
        if($arraySubAcao){
            foreach($arraySubAcao as $subid){
                if($arrayCombos){
                    foreach($arrayCombos as $combos){
                        $combosSelecionadas = explode(":",$combos);
                        if($combosSelecionadas[0]==$subid){
                            $subacoesSelecionadas .=($subacoesSelecionadas!="")?",":"";
                            $sql = "";
                            if($this->eof("select scoid from monitoraseb.subacaocoordenacao where sbaid = $subid and coonid = $coonid")){
                                $sql = "insert into monitoraseb.subacaocoordenacao
                                            (coonid, sbaid, usucpf)
                                        values ($coonid, $subid, '$combosSelecionadas[1]')
                                        returning scoid";

                                $scoid = $this->pegaUm($sql);

                                $docid = wf_cadastrarDocumento(WORKFLOW_MONITORAMENTO_SEB, "Fluxo da SubAção x Coordenação - ID $scoid");

                                $sql = "update monitoraseb.subacaocoordenacao
                                           set docid = $docid
                                         where scoid = $scoid";
                            }else{
                                $sql = "update monitoraseb.subacaocoordenacao
                                           set usucpf='$combosSelecionadas[1]'
                                         where sbaid = $subid
                                           AND coonid = $coonid";
                            }
                            $this->executar($sql);
                            $subacoesSelecionadas .= $subid;
                        }
                    }
                }
            }
        }
        $erros = $this->removeSubacoesNaoSelecionadas($subacoesSelecionadas, $coonid);
        if(!empty($erros)){
            $this->rollback();
            return $erros;
        }

        return "";
    }


    private function removeSubacoesNaoSelecionadas($subacoesSelecionadas, $coonid){
        $condicao = ($subacoesSelecionadas=="")?"":"sbaid not in ($subacoesSelecionadas) and";
        if($this->eof("select sc.* FROM monitoraseb.subacaocoordenacao sc JOIN monitoraseb.cursomestre cm ON (sc.scoid = cm.scoid) WHERE ".$condicao." coonid=".$coonid)){
            $sql = "delete from monitoraseb.subacaocoordenacao
                     where $condicao coonid=$coonid;
                    delete from monitoraseb.subacaopublicoalvo
                     where sbaid not in (select sbaid from monitoraseb.subacaocoordenacao);
                    delete from monitoraseb.subacaoanexo
                     where sbaid not in (select sbaid from monitoraseb.subacaocoordenacao);";

            $this->executar($sql);
            return "";
        }else{
            return "Operação não realizada. Subação vinculada ao Curso Mestre.";
        }

    }

    private function verificaVinculo($coonid, $subId){
        $sql = "SELECT usucpf FROM monitoraseb.subacaocoordenacao WHERE sbaid = ".$subId." AND coonid =". $coonid;
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem[0]['usucpf'] : "";
    }

    public function listarAcoes($subId){
        $sql = "SELECT distinct(aca.acadsc) as descricao FROM monitora.acao aca
                 inner JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
                 inner JOIN monitora.pi_subacaodotacao sd ON dtl.ptrid = sd.ptrid
                 inner JOIN public.unidade uni ON uni.unicod = dtl.unicod
                WHERE  sd.sbaid = ".$subId." order by aca.acadsc";
        $listagem = $this->carregar($sql);
        return ($listagem) ? $listagem : array();
    }

}