<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

?>
<div class="row">
    <div class="col-lg-12">
        <div class="page-header">
            <h3 id="forms">
<!--                PDU --->
                Plano de Desenvolvimento das Universidades Federais
<!--                <small>-->
<!--                    Plano de desenvolvimento das universidades federais-->
<!--                </small>-->
            </h3>
        </div>
    </div>
</div>
<?php
    $controllerInstituicao = new Controller_Instituicao();
    $controllerInstituicao->pesquisarAction();
?>